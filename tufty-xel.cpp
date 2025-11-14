#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/platform.h"

#include <unordered_map>
#include <functional>

#include "libraries/tufty2040/tufty2040.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

#include "include/scenes/sonaScene.hpp"
#include "include/layers/bearPawBackgroundLayer.hpp"

using namespace pimoroni;

int main()
{
    // Tufty is 320 x 240 with 133mhz cpu with 264kb of sram
    Tufty2040 tufty{};
    PicoGraphics_PenRGB556 graphics{Tufty2040::WIDTH, Tufty2040::HEIGHT, nullptr};

    stdio_init_all();

    SonaScene sonaScene{&graphics};

    // Unordered map to match a key type to a lambda of the tufty function to fetch that key
    // Loop over the map, generate all instances of events for this update, pass them to
    // in progress scene manager.
    std::unordered_map<Xel::ButtonEvent::Button, std::function<bool()>> buttonMap {
        {Xel::ButtonEvent::Button::A, [&tufty](){ return tufty.readA(); }},
        {Xel::ButtonEvent::Button::B, [&tufty](){ return tufty.readB(); }},
        {Xel::ButtonEvent::Button::C, [&tufty](){ return tufty.readC(); }},
        {Xel::ButtonEvent::Button::UP, [&tufty](){ return tufty.readUp(); }},
        {Xel::ButtonEvent::Button::DOWN, [&tufty](){ return tufty.readDown(); }},
    };

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
