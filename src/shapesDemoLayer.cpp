#include "include/layers/shapesDemoLayer.hpp"

ShapesDemoLayer::ShapesDemoLayer(pimoroni::PicoGraphics* gc, uint8_t shapeCount) : Layer{gc}
{
    shapes.reserve(shapeCount);

    if (!gc) { return; }

    shapePoint shape{};
    for(size_t i = 0; i < shapeCount; ++i) {
        shape.x = rand() % gc->bounds.w;
        shape.y = rand() % gc->bounds.h;
        shape.r = (rand() % 10) + 3;
        shape.dx = float(rand() % 255) / 64.0f;
        shape.dy = float(rand() % 255) / 64.0f;
        shape.pen = gc->create_pen(rand() % 255, rand() % 255, rand() % 255);
        shapes.emplace_back(shape);
    }
}

void ShapesDemoLayer::update(double dt, const std::vector<Xel::LayerData*> &layerData)
{
    if (!context) { return; }

    for(auto& shape : shapes) {
        shape.x += shape.dx;
        shape.y += shape.dy;
        if((shape.x - shape.r) < 0) {
        shape.dx *= -1;
        shape.x = shape.r;
        }
        if((shape.x + shape.r) >= context->bounds.w) {
        shape.dx *= -1;
        shape.x = context->bounds.w - shape.r;
        }
        if((shape.y - shape.r) < 0) {
        shape.dy *= -1;
        shape.y = shape.r;
        }
        if((shape.y + shape.r) >= context->bounds.h) {
        shape.dy *= -1;
        shape.y = context->bounds.h - shape.r;
        }

        context->set_pen(shape.pen);
        context->circle(pimoroni::Point(shape.x, shape.y), shape.r);
    }   
}
