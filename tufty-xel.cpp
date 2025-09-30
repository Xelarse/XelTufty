#include "pico/stdlib.h"
#include <stdio.h>
#include <iostream>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <chrono>
#include "pico/time.h"
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "drivers/st7789/st7789.hpp"
#include "drivers/button/button.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/tufty2040/tufty2040.hpp"

#include "include/images/bearIcon100x100.h"
#include "include/images/bearIcon200x200.h"
#include "include/layers/shapesDemoLayer.hpp"
#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/layers/imageLayer.hpp"

using namespace pimoroni;

// Tufty is 320 x 240 with 133mhz cpu with 264kb of sram
// input range is 3v - 5.5v

Tufty2040 tufty;

ST7789 st7789(
    Tufty2040::WIDTH,
    Tufty2040::HEIGHT,
    ROTATE_0,
    ParallelPins{
        Tufty2040::LCD_CS,
        Tufty2040::LCD_DC,
        Tufty2040::LCD_WR,
        Tufty2040::LCD_RD,
        Tufty2040::LCD_D0,
        Tufty2040::BACKLIGHT});

struct textRenderInfo
{
    std::string text = "";
    Point pos{310, 100};
    int wrapPixelCount = 100;
    float scale = 6.0f;
    float angle = 180.f;
    uint8_t spacing = 1;
};

PicoGraphics_PenRGB556 graphics(st7789.width, st7789.height, nullptr);

Button button_a(Tufty2040::A, Polarity::ACTIVE_HIGH);
Button button_b(Tufty2040::B, Polarity::ACTIVE_HIGH);
Button button_c(Tufty2040::C, Polarity::ACTIVE_HIGH);
Button button_up(Tufty2040::UP, Polarity::ACTIVE_HIGH);
Button button_down(Tufty2040::DOWN, Polarity::ACTIVE_HIGH);

// auto centerText(PicoGraphics_PenRGB556& graphics, textRenderInfo& textRenderInfo, int scrWidth) -> void {
//   // FIXME: rendering this isnt working as expected cout the values and check tomoz, maybe hersshey needs something special?
//   auto textWidth = graphics.measure_text(textRenderInfo.text, textRenderInfo.scale, textRenderInfo.spacing, false);
//   textRenderInfo.pos.x = (scrWidth * 0.5f) - (textWidth * 0.5f);
// }

// template<class Resolution, class Step>
// auto timestampDifference(const std::chrono::steady_clock::time_point& before, const std::chrono::steady_clock::time_point& after) -> double {
//   std::chrono::duration<Resolution, Step> delta{after - before};
//   return delta.count();
// }

int main()
{
    stdio_init_all();
    st7789.set_backlight(255);
    graphics.set_font("bitmap8");

    BearPawBackgroundLayer bpb{&graphics};
    ShapesDemoLayer sdl{&graphics, 20};
    ImageLayer bearImage200{&graphics, const_cast<uint8_t*>(bearIconAlpha200x200), sizeof(bearIconAlpha200x200)};
    ImageLayer bearImage100{&graphics, const_cast<uint8_t*>(bearIconAlpha100x100), sizeof(bearIconAlpha100x100)};


    Xel::ImageData bearIconData{};
    bearIconData.type = Xel::LayerDataType::IMAGE;
    bearIconData.x = 100;
    bearIconData.y = 0;

    while (true)
    {
        bpb.update();
        bearImage200.update(reinterpret_cast<Xel::LayerData*>(&bearIconData));
        bearImage100.update(reinterpret_cast<Xel::LayerData*>(&bearIconData));
        sdl.update();
        st7789.update(&graphics);
    }

    //   bool needsRedraw = true;
    //   while(true) {
    //     if (needsRedraw) {
    //       textRenderInfo tInfo{.text = "Xels Tufty"};
    //       graphics.set_pen(0,0,0);
    //       graphics.text(tInfo.text, tInfo.pos, tInfo.wrapPixelCount, tInfo.scale, tInfo.angle, tInfo.spacing);
    //       // update screen
    //       st7789.update(&graphics);
    //       needsRedraw = false;
    //     }

    //     // FIXME: This borks console output, math is mathin but cant prove it out until I have moving stuff
    //     //Force a 1 fps limit to reduce battery drain
    //     static const double forcedMinWaitMs{100e6};
    //     auto delta = forcedMinWaitMs - (renderTs - startTs).count();
    //     if (delta > 0.0) {
    //       // using namespace std::chrono_literals;
    //       // std::this_thread::sleep_for(delta * 1ms);
    //       busy_wait_ms(delta);
    //     }
    //     auto sleepTs{std::chrono::steady_clock::now()};

    //     const auto startTs{std::chrono::steady_clock::now()};
    //     auto processingTs{std::chrono::steady_clock::now()};
    //     auto renderTs{std::chrono::steady_clock::now()};
    //     std::cout <<
    //     "Compute: "   << timestampDifference<double, std::milli>(startTs, processingTs) <<
    //     "ms Render: " << timestampDifference<double, std::milli>(processingTs, renderTs) <<
    //     // "ms Sleep: "  << timestampDifference<double, std::milli>(renderTs, sleepTs) <<
    //     "ms Total: "  << timestampDifference<double, std::milli>(startTs, renderTs) << "ms" << std::endl;
    //   }

    return 0;
}
