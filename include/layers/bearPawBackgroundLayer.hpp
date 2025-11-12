#pragma once

#include "libraries/xel-present/layer.hpp"

class BearPawBackgroundLayer final : public Xel::Layer {
    public:
        BearPawBackgroundLayer(pimoroni::PicoGraphics* gc) : Layer{gc} {};
        
        auto update(Xel::LayerData* layerData = nullptr) -> void override;

    private:
        BearPawBackgroundLayer() = default;
};