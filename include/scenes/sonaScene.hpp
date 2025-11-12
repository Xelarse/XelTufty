#pragma once

#include <vector>
#include <variant>

#include "libraries/xel-present/scene.hpp"
#include "libraries/xel-present/layer.hpp"
#include "include/layers/bearPawBackgroundLayer.hpp"
#include "include/layers/imageLayer.hpp"
#include "include/layers/textLayer.hpp"

class SonaScene final : public Xel::Scene {
    public:
        SonaScene(pimoroni::PicoGraphics* gc);

        auto update(Xel::Event* eventChain = nullptr) -> void override;

    private:
        SonaScene() = default;

        // Predefine layer variants to avoid additional heap allocations for indirection.
        using LayerVariant = std::variant<BearPawBackgroundLayer, ImageLayer, TextLayer>;
        std::vector<LayerVariant> renderLayers{};
};