#pragma once

#include "libraries/xel-present/layer.hpp"

class BearPawBackgroundLayer final : public Xel::Layer {
    public:
        BearPawBackgroundLayer(pimoroni::PicoGraphics* gc) : Layer{gc} {};
        
        auto update(Xel::LayerData* layerData = nullptr) -> void override;

        auto darkMode(bool isActive) -> void { darkmode = isActive; };

    private:
        BearPawBackgroundLayer() = default;

        bool darkmode = false;
        const Xel::RGB primary{Xel::bearFlesh.r, Xel::bearFlesh.g, Xel::bearFlesh.b};
        const Xel::RGB secondary{Xel::bearNose.r, Xel::bearNose.g, Xel::bearNose.b};
};