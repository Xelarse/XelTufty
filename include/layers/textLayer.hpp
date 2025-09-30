#pragma once

#include "layer.hpp"

class TextLayer final : public Xel::Layer {
    public:
        TextLayer(pimoroni::PicoGraphics* gc) : Layer{gc} {};
        TextLayer(pimoroni::PicoGraphics* gc, Xel::LayerData* layerData);
        ~TextLayer() override = default;

        auto update(Xel::LayerData* layerData = nullptr) -> void override;

    private:
        TextLayer() = default;

        auto processDataChain(Xel::LayerData* layerData) -> void;

        Xel::TextData lastTextData{};
        Xel::PositionData lastPositionData{};
};

// auto centerText(PicoGraphics_PenRGB556& graphics, textRenderInfo& textRenderInfo, int scrWidth) -> void {
//   // FIXME: rendering this isnt working as expected 
//   auto textWidth = graphics.measure_text(textRenderInfo.text, textRenderInfo.scale, textRenderInfo.spacing, false);
//   textRenderInfo.pos.x = (scrWidth * 0.5f) - (textWidth * 0.5f);
// }