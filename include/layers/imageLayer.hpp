#pragma once

#include "layer.hpp"
#include "libraries/pngdec/PNGdec.h"

// Layer that utilises pngdec to load a given image. Only supports PNGS rn.
class ImageLayer final : public Xel::Layer {
    public:
        // TODO: Add constructor to also load image data from a file.
        ImageLayer(pimoroni::PicoGraphics* gc, uint8_t* pngData, int pngDataSize);
        ~ImageLayer() override;

        auto update(Xel::LayerData* layerData = nullptr) -> void override;

    private:
        ImageLayer() = default;

        auto static pngDrawCallback(PNGDRAW* pDraw) -> int;

        PNG png;
        int pngOpenStatus = -1;
        Xel::ImageData lastImageData{};
        Xel::PositionData lastPositionData{};
};