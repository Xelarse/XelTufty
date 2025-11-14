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
        const Xel::RGB primary{Xel::RGB_CONSTS::BEAR_FLESH};
        const Xel::RGB secondary{Xel::RGB_CONSTS::BEAR_NOSE};
};