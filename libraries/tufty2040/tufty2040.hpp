#pragma once

#include <math.h>
#include <tuple>

#include "hardware/gpio.h"  // Workaround SDK bug - https://github.com/raspberrypi/pico-sdk/issues/3
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "drivers/button/button.hpp"
#include "drivers/st7789/st7789.hpp"

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

    static constexpr uint8_t VBAT_ADC = 29;
    static constexpr uint8_t LUX_ADC = 26;
    static constexpr uint8_t VBAT_ADC_INPUT = 3;
    static constexpr uint8_t LUX_ADC_INPUT = 0;
    static constexpr uint8_t LUX_SENSOR_POWER = 27;
    static constexpr uint8_t USB_POWER = 24;


    Tufty2040();
    ~Tufty2040() = default;

    auto led(uint8_t brightness) -> void;
    auto readAmbientLightScale() -> uint16_t;
    auto readBattery() -> std::tuple<double, float, bool>; // <rawVoltage, batteryPercent, usbPowered>
    auto update() -> void;
    auto render(pimoroni::PicoGraphics* context) -> void;
    inline auto readA() -> bool { return buttonA.raw(); }
    inline auto readB() -> bool { return buttonB.raw(); }
    inline auto readC() -> bool { return buttonC.raw(); }
    inline auto readUp() -> bool { return buttonUp.raw(); }
    inline auto readDown() -> bool { return buttonDown.raw(); }

  private:
    Button buttonA{A, Polarity::ACTIVE_HIGH, 0};
    Button buttonB{B, Polarity::ACTIVE_HIGH, 0};
    Button buttonC{C, Polarity::ACTIVE_HIGH, 0};
    Button buttonUp{UP, Polarity::ACTIVE_HIGH, 0};
    Button buttonDown{DOWN, Polarity::ACTIVE_HIGH, 0};
    ST7789 st7789{ WIDTH, HEIGHT, ROTATE_0, ParallelPins{ LCD_CS, LCD_DC, LCD_WR, LCD_RD, LCD_D0, BACKLIGHT }};

    auto autoBrightness() -> uint8_t;
    auto calculateVoltageScale(double voltage) -> float;    
  };
}
