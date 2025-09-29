#pragma once

#include "layer.hpp"

class BearPawBackgroundLayer final : public Xel::Layer {
    public:
        BearPawBackgroundLayer(pimoroni::PicoGraphics* gc) : Layer{gc} {};
        ~BearPawBackgroundLayer() override = default;

        auto update(double dt = .0, const std::vector<Xel::LayerData*>& layerData = {}) -> void override;

    private:
        BearPawBackgroundLayer() = default;
};