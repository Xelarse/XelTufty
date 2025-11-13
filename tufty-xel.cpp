#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/platform.h"

#include "libraries/tufty2040/tufty2040.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

#include "include/scenes/sonaScene.hpp"
#include "include/layers/bearPawBackgroundLayer.hpp"

using namespace pimoroni;

// Tufty is 320 x 240 with 133mhz cpu with 264kb of sram
Tufty2040 tufty{};
PicoGraphics_PenRGB556 graphics{Tufty2040::WIDTH, Tufty2040::HEIGHT, nullptr};

int main()
{
    stdio_init_all();

    SonaScene sonaScene{&graphics};

    while (true)
    {
        Xel::AmbientLightEvent lightEvent{};
        lightEvent.scale = tufty.readAmbientLightScale();

        sonaScene.update(reinterpret_cast<Xel::Event*>(&lightEvent));
        tufty.update();
        tufty.render(&graphics);
        // needsRedraw = false;
    }

    return 0;
}

// TODO: sleep logic to save power
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
