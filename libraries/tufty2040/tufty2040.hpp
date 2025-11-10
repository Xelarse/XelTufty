#pragma once

#include <string>
#include <math.h>
#include <tuple>

#include "hardware/gpio.h"  // Workaround SDK bug - https://github.com/raspberrypi/pico-sdk/issues/3
#include "hardware/pwm.h"
#include "hardware/adc.h"

namespace pimoroni {

  class Tufty2040 {
  public:
    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 240;
    static constexpr uint8_t A = 7;
    static constexpr uint8_t B = 8;
    static constexpr uint8_t C = 9;
    static constexpr uint8_t UP = 22;
    static constexpr uint8_t DOWN = 6;
    static constexpr uint8_t LED = 25;
    static constexpr uint8_t BACKLIGHT = 2;

    static constexpr uint8_t LCD_CS = 10;
    static constexpr uint8_t LCD_DC = 11;
    static constexpr uint8_t LCD_WR = 12;
    static constexpr uint8_t LCD_RD = 13;
    static constexpr uint8_t LCD_D0 = 14;

  private:
    static constexpr uint8_t vbat_adc = 29;
    static constexpr uint8_t vref_adc = 28;
    static constexpr uint8_t lux_adc = 26;
    static constexpr uint8_t vbat_input = 3;
    static constexpr uint8_t vref_input = 2;
    static constexpr uint8_t lux_input = 0;
    static constexpr uint8_t vref_en = 27;
    static constexpr uint8_t usb_power = 24;

    static constexpr float LOW_BATTERY_VOLTAGE = 3.2f;
    static constexpr float FULL_BATTERY_VOLTAGE = 4.5f;  // Based from 3x1.5v AAAs

  public:
    Tufty2040() {
      
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
      gpio_set_dir(vref_en, true);    // sets vref to a pin.OUT so we can set it for reading later
      gpio_set_dir(usb_power, false); // sets usb power pin to pin.IN so we can read it for usb powering vs bat.
      adc_init();                     // Init the ADC interface
      adc_gpio_init(vbat_adc);        // Initialise our adc's for reading later
      adc_gpio_init(vref_adc);
    }

    void led(uint8_t brightness) {
      // set the led brightness from 1 to 256 with gamma correction
      float gamma = 2.8;
      uint16_t v = (uint16_t)(pow((float)(brightness) / 256.0f, gamma) * 65535.0f + 0.5f);
      pwm_set_gpio_level(LED, v);
    }

    uint16_t luminance() {
        // Enable vref reading
        gpio_pull_up(vref_en);

        // Read out lux val
        adc_select_input(lux_input);
        const auto luxRead = adc_read();

        // Disable vref reading
        gpio_pull_down(vref_en);

        return luxRead;
    }

    // tuple in the format <rawVoltage, batteryPercent, usbPowered>
    std::tuple<double, float, bool> readBattery() {
        // Enable vref reading
        gpio_pull_up(vref_en);

        // Read out vref and vbat vals
        adc_select_input(vref_input);
        const auto vrefRead = adc_read();

        adc_select_input(vbat_input);
        const auto vbatRead = adc_read();

        // Disable vref reading
        gpio_pull_down(vref_en);

        // This still makes next to no sense to me so dropping in the pimoroni's comments
        // TODO research this stuff further

        // Calculate the logic supply voltage, as will be lower that the usual 3.3V when running off low batteries
        const auto vdd = 1.24 * (65535 / static_cast<double>(vrefRead));

        // 3 in this is a gain, not rounding of 3.3V
        const auto vbat = (static_cast<double>(vbatRead) / 65535) * 3.0 * vdd;

        return {vbat, calculatePercentage(vbat), gpio_get(usb_power)};
    }

  private:
    float calculatePercentage(double voltage) {
      return 100.f * ((voltage - LOW_BATTERY_VOLTAGE) / (FULL_BATTERY_VOLTAGE - LOW_BATTERY_VOLTAGE));
    }
  };
}
