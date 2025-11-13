#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/utilities.hpp"

auto BearPawBackgroundLayer::update(Xel::LayerData* layerData) -> void
{
    if (!context) { return; }

    const auto& bg = !darkmode ? primary : secondary;
    context->set_pen(bg.r, bg.g, bg.b);
    context->clear();


    int screenWidth = context->bounds.w;
    int screenHeight = context->bounds.h;

    const auto& accent = !darkmode ? secondary : primary;
    context->set_pen(accent.r, accent.g, accent.b);

    // Middle Base
    context->circle(pimoroni::Point{screenWidth/2, -40}, 100);

    // Side beans
    context->circle(pimoroni::Point{screenWidth/2 - (screenWidth/4 + 20), 80}, 50);
    context->circle(pimoroni::Point{screenWidth/2 + (screenWidth/4 + 20), 80}, 50);

    // upper Bean
    context->circle(pimoroni::Point{screenWidth/2, 120}, 50);
}
