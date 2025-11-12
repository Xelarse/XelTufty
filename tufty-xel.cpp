#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "libraries/tufty2040/tufty2040.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

#include "include/images/icon240x240.h"
#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/layers/imageLayer.hpp"
#include "include/layers/textLayer.hpp"

using namespace pimoroni;

// Tufty is 320 x 240 with 133mhz cpu with 264kb of sram
Tufty2040 tufty;
PicoGraphics_PenRGB556 graphics(Tufty2040::WIDTH, Tufty2040::HEIGHT, nullptr);

int main()
{
    using namespace Xel;

    stdio_init_all();
    
    PositionData bearIconPosition{};
    bearIconPosition.x = 0;
    bearIconPosition.y = 0;

    ImageData bearIconImageData{};
    bearIconImageData.mirrorFlags = ImageData::MirrorFlags::MIRROR_X | ImageData::MirrorFlags::MIRROR_Y;
    bearIconImageData.next = reinterpret_cast<LayerData*>(&bearIconPosition);

    PositionData textPosition{};
    textPosition.x = 310;
    textPosition.y = 230;

    TextData textData{};
    textData.next = reinterpret_cast<LayerData*>(&textPosition);
    textData.text = "Xel";
    textData.scale = 6.8f;
    textData.rotation = 180.f;

    BearPawBackgroundLayer bpb{&graphics};
    ImageLayer bearImage{&graphics, const_cast<uint8_t*>(Icon240x240), sizeof(Icon240x240)};
    TextLayer xelText{&graphics, reinterpret_cast<LayerData*>(&textData)};

    bool needsRedraw = true;
    while (true)
    {
        if (needsRedraw)
        {
            bpb.update();
            bearImage.update(reinterpret_cast<LayerData*>(&bearIconImageData));
            xelText.update();
            tufty.update();
            tufty.render(&graphics);
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
