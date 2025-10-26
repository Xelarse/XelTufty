#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "drivers/st7789/st7789.hpp"
#include "drivers/button/button.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/tufty2040/tufty2040.hpp"

#include "include/images/bearIcon200x200.h"
#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/layers/imageLayer.hpp"
#include "include/layers/textLayer.hpp"

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
        Tufty2040::BACKLIGHT
    }
);

PicoGraphics_PenRGB556 graphics(st7789.width, st7789.height, nullptr);

Button button_a(Tufty2040::A, Polarity::ACTIVE_HIGH);
Button button_b(Tufty2040::B, Polarity::ACTIVE_HIGH);
Button button_c(Tufty2040::C, Polarity::ACTIVE_HIGH);
Button button_up(Tufty2040::UP, Polarity::ACTIVE_HIGH);
Button button_down(Tufty2040::DOWN, Polarity::ACTIVE_HIGH);

int main()
{
    using namespace Xel;

    stdio_init_all();
    st7789.set_backlight(255);
    
    PositionData bearIconPosition{};
    bearIconPosition.x = 0;
    bearIconPosition.y = 0;

    ImageData bearIconImageData{};
    //TODO work out a better interface for these flags it feels shit to use like this.
    bearIconImageData.mirrorFlags = static_cast<ImageData::MirrorFlags>(ImageData::MirrorFlags::MIRROR_X | ImageData::MirrorFlags::MIRROR_Y);
    bearIconImageData.next = reinterpret_cast<LayerData*>(&bearIconPosition);

    PositionData textPosition{};
    textPosition.x = 310;
    textPosition.y = 230;

    TextData textData{};
    textData.next = reinterpret_cast<LayerData*>(&textPosition);
    textData.text = "Xel";
    textData.scale = 6.2f;
    textData.rotation = 180.f;

    BearPawBackgroundLayer bpb{&graphics};
    ImageLayer bearImage{&graphics, const_cast<uint8_t*>(bearIconAlpha200x200), sizeof(bearIconAlpha200x200)};
    TextLayer xelText{&graphics, reinterpret_cast<LayerData*>(&textData)};

    bool needsRedraw = true;
    while (true)
    {
        if (needsRedraw)
        {
            bpb.update();
            bearImage.update(reinterpret_cast<LayerData*>(&bearIconImageData));
            xelText.update();
            st7789.update(&graphics);
            // needsRedraw = false;
        }
        // TODO: sleep logic here to save power
    }

    return 0;
}


// Timer WIP----------------------------------------------
// template<class Resolution, class Step>
// auto timestampDifference(const std::chrono::steady_clock::time_point& before, const std::chrono::steady_clock::time_point& after) -> double {
//   std::chrono::duration<Resolution, Step> delta{after - before};
//   return delta.count();
// }
//
//     // FIXME: This borks console output, math is mathin but cant prove it out until I have moving stuff
//     //Force a 1 fps limit to reduce battery drain
//     static const double forcedMinWaitMs{100e6};
//     auto delta = forcedMinWaitMs - (renderTs - startTs).count();
//     if (delta > 0.0) {
//       using namespace std::chrono_literals;
//       std::this_thread::sleep_for(delta * 1ms);
//       busy_wait_ms(delta);
//     }
//     auto sleepTs{std::chrono::steady_clock::now()};

//     const auto startTs{std::chrono::steady_clock::now()};
//     auto processingTs{std::chrono::steady_clock::now()};
//     auto renderTs{std::chrono::steady_clock::now()};
//     std::cout <<
//     "Compute: "   << timestampDifference<double, std::milli>(startTs, processingTs) <<
//     "ms Render: " << timestampDifference<double, std::milli>(processingTs, renderTs) <<
//     "ms Sleep: "  << timestampDifference<double, std::milli>(renderTs, sleepTs) <<
//     "ms Total: "  << timestampDifference<double, std::milli>(startTs, renderTs) << "ms" << std::endl;
//   }
