#pragma once

#include "libraries/xel-present/layer.hpp"
#include <vector>

class ShapesDemoLayer final : public Xel::Layer {
    public:
        ShapesDemoLayer(pimoroni::PicoGraphics* gc, uint8_t shapeCount);
        ~ShapesDemoLayer() override = default;

        auto update(Xel::LayerData* layerData = nullptr) -> void override;

    private:
        struct shapePoint {
            float      x;
            float      y;
            uint8_t    r;
            float     dx;
            float     dy;
            uint16_t pen;
        };

        ShapesDemoLayer() = default;

        std::vector<shapePoint> shapes;
};