#include "include/layers/imageLayer.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

ImageLayer::ImageLayer(pimoroni::PicoGraphics* gc, uint8_t* pngData, int pngDataSize) : Layer{gc}
{
    pngOpenStatus = png.openRAM(pngData, pngDataSize, ImageLayer::pngDrawCallback);
}

ImageLayer::~ImageLayer()
{
    png.close();
}

auto ImageLayer::update(Xel::LayerData* layerData) -> void
{
    if (!context) { return; }
    if (pngOpenStatus != PNG_SUCCESS) { return; }

    // TODO: add struct chain traversal helper when its written
    if (layerData->type == Xel::LayerDataType::IMAGE) {
        lastImageData = *(reinterpret_cast<Xel::ImageData*>(layerData));
    }

    png.decode(reinterpret_cast<void*>(this), 0);
}

auto ImageLayer::pngDrawCallback(PNGDRAW *pDraw) -> int
{
    ImageLayer* instance = reinterpret_cast<ImageLayer*>(pDraw->pUser);

    if (!instance->context) { return 0; }
    if (instance->pngOpenStatus != PNG_SUCCESS) { return 0; }


    // TODO: Add transparency, boarder and mirroring via the lastImageData settings. 
    std::vector<uint16_t> lineBuffer;
    lineBuffer.resize(pDraw->iWidth * sizeof(uint16_t));
    instance->png.getLineAsRGB565(pDraw, lineBuffer.data(), PNG_RGB565_BIG_ENDIAN, 0xffffffff);

    uint16_t* currentPixel = lineBuffer.data();
    for(int x = 0; x < pDraw->iWidth; ++x) {
        auto rgbVal = pimoroni::RGB565(*currentPixel);
        instance->context->set_pixel_dither(
            pimoroni::Point{
                instance->lastImageData.x + x, 
                instance->lastImageData.y + instance->lastImageData.y + pDraw->y}, 
            rgbVal);
        ++currentPixel;
    }
    
    // For some reason returning a PNG_SUCCESS (which is 0) causes the library to EARLY quit, 
    // lowkey dumb but changed to allow for processing of this func
    return 1;
}
