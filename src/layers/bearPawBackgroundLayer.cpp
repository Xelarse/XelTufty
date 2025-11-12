#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/utilities.hpp"

auto BearPawBackgroundLayer::update(Xel::LayerData* layerData) -> void
{
    if (!context) { return; }

    // FIXME:: gotta be a better way to map these values? Maybe concepts after figuring out unsigned int size vs 3 uint8s
    context->set_pen(Xel::bearFlesh.r, Xel::bearFlesh.g, Xel::bearFlesh.b);
    context->clear();


    int screenWidth = context->bounds.w;
    int screenHeight = context->bounds.h;

    context->set_pen(Xel::bearNose.r, Xel::bearNose.g, Xel::bearNose.b);

    // Middle Base
    context->circle(pimoroni::Point{screenWidth/2, -40}, 100);

    // Side beans
    context->circle(pimoroni::Point{screenWidth/2 - (screenWidth/4 + 20), 80}, 50);
    context->circle(pimoroni::Point{screenWidth/2 + (screenWidth/4 + 20), 80}, 50);

    // upper Bean
    context->circle(pimoroni::Point{screenWidth/2, 120}, 50);
}
