#pragma once

#include "layer.hpp"

class BearPawBackgroundLayer final : public Xel::Layer {
    public:
        BearPawBackgroundLayer(pimoroni::PicoGraphics* gc) : Layer{gc} {};
        ~BearPawBackgroundLayer() override = default;

        auto update(Xel::LayerData* layerData = nullptr) -> void override;

    private:
        BearPawBackgroundLayer() = default;
};