#include <math.h>
#include <string.h>

#include "tufty2040.hpp"

namespace pimoroni {
    Tufty2040::Tufty2040()
    {
        gpio_set_function(LCD_D0 + 0, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 0, true);
        gpio_set_function(LCD_D0 + 1, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 1, true);
        gpio_set_function(LCD_D0 + 2, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 2, true);
        gpio_set_function(LCD_D0 + 3, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 3, true);
        gpio_set_function(LCD_D0 + 4, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 4, true);
        gpio_set_function(LCD_D0 + 5, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 5, true);
        gpio_set_function(LCD_D0 + 6, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 6, true);
        gpio_set_function(LCD_D0 + 7, GPIO_FUNC_SIO); gpio_set_dir(LCD_D0 + 7, true);

        // led control pin
        pwm_config cfg = pwm_get_default_config();
        pwm_set_wrap(pwm_gpio_to_slice_num(LED), 65535);
        pwm_init(pwm_gpio_to_slice_num(LED), &cfg, true);
        gpio_set_function(LED, GPIO_FUNC_PWM);
        led(0);

        // Power related preamble
        gpio_set_dir(LUX_SENSOR_POWER, true); // sets vref to a pin.OUT so we can set it for reading later
        gpio_set_dir(USB_POWER, false);       // sets usb power pin to pin.IN so we can read it for usb powering vs bat.
        adc_init();                           // Init the ADC interface
        adc_gpio_init(VBAT_ADC);              // Initialise our adc's for reading later
        adc_gpio_init(LUX_ADC);        
    }

    auto Tufty2040::led(uint8_t brightness) -> void {
        // set the led brightness from 1 to 256 with gamma correction
        float gamma = 2.8;
        uint16_t v = (uint16_t)(pow((float)(brightness) / 256.0f, gamma) * 65535.0f + 0.5f);
        pwm_set_gpio_level(LED, v);
    }

    auto Tufty2040::readAmbientLightScale() -> uint16_t
    {
        static constexpr uint8_t LOW_LUX = 60;
        static constexpr uint8_t HIGH_LUX = 73;

        const auto calcScale = [](float val) -> float {
            return std::min(std::max(0.0f, (val - LOW_LUX) / (HIGH_LUX - LOW_LUX)), 1.0f);
        };

        // Enable the sensor for reading.
        gpio_pull_up(LUX_SENSOR_POWER);

        // Read out lux val
        adc_select_input(LUX_ADC_INPUT);
        const auto luxRead = adc_read();

        // Disable the sensor after reading
        gpio_pull_down(LUX_SENSOR_POWER);

        return calcScale(luxRead);
    }

    auto Tufty2040::readBattery() -> std::tuple<double, float, bool>
    {
        adc_select_input(VBAT_ADC_INPUT);
        const auto vbatRead = adc_read();

        // Vbat read is a 12-bit reading for a %age of the reference.
        // Our reference is 3.3v so we multi our interpolation by the reference to figure out v.
        // constant multi of 3 to account for resistors in line with the ADC to prevent overvolting chip.
        const auto vbat = (static_cast<double>(vbatRead) / 4095.0) * 3.3 * 3.0;

        return {vbat, calculateVoltageScale(vbat), gpio_get(USB_POWER)};
    }

    auto Tufty2040::update() -> void
    {
        st7789.set_backlight(autoBrightness());
    }

    auto Tufty2040::render(pimoroni::PicoGraphics* context) -> void
    {
        st7789.update(context);
    }

    auto Tufty2040::autoBrightness() -> uint8_t {
        // Backlight range is from 100-255, lower than 100 seems to just black screen.
        static constexpr uint8_t BACKLIGHT_LOW = 200;
        static constexpr uint8_t BACKLIGHT_HIGH = 255;
        static float previousBacklight = 0;

        const auto ambientScale = readAmbientLightScale();

        const auto backlightLerp = [](float t) -> float {
            return BACKLIGHT_LOW + (BACKLIGHT_HIGH - BACKLIGHT_LOW) * t;
        };

        // On bootup to save waiting for the light to change just set it immediately.
        if (previousBacklight == 0) {
            previousBacklight = backlightLerp(ambientScale);
            return previousBacklight;
        }

        // Use the previous value to smooth out changes to reduce flickering.
        const float backlight_diff = backlightLerp(ambientScale) - previousBacklight;
        const float backlight = previousBacklight + (backlight_diff * (1.f / 2.f)); // 2 affects smoothing, lower number = faster
        previousBacklight = backlight;

        return backlight;
    }    

    auto Tufty2040::calculateVoltageScale(double voltage) -> float
    {
        // Based from 3x1.5v AAAs. Although these batteries are lithium batteries that hold a pretty
        // consistent voltage during discharge apparently vs the 1.2v nimh batteries ive got.
        // Debating weighing up the cost of a consistent ~1.5x3v for the entire runtime with no warning to low bat
        // vs a lower output voltage but a more gradual discharge curve of nimh 1.2v cells.
        static constexpr float LOW_BATTERY_VOLTAGE = 3.0f;
        static constexpr float FULL_BATTERY_VOLTAGE = 4.5f;

        return 100.f * ((voltage - LOW_BATTERY_VOLTAGE) / (FULL_BATTERY_VOLTAGE - LOW_BATTERY_VOLTAGE));
    }
}