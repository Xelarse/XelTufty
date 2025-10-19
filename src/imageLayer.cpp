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
    auto currentLayer = layerData;

    while(currentLayer) {
        switch(currentLayer->type) {
            case Xel::LayerDataType::IMAGE: {
                lastImageData = *(reinterpret_cast<Xel::ImageData*>(currentLayer));
                break;
            }
            case Xel::LayerDataType::POSITION: {
                lastPositionData = *(reinterpret_cast<Xel::PositionData*>(currentLayer));
                break;
            }
        }

        currentLayer = currentLayer->next;
    }

    png.decode(reinterpret_cast<void*>(this), 0);
}

auto ImageLayer::pngDrawCallback(PNGDRAW *pDraw) -> int
{
    ImageLayer* instance = reinterpret_cast<ImageLayer*>(pDraw->pUser);

    if (!instance->context) { return 0; }
    if (instance->pngOpenStatus != PNG_SUCCESS) { return 0; }

    // Fetch the full line relating to this render.
    std::vector<uint16_t> lineBuffer;
    lineBuffer.resize(pDraw->iWidth * sizeof(uint16_t));
    instance->png.getLineAsRGB565(pDraw, lineBuffer.data(), PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    uint16_t* currentPixel = lineBuffer.data();

    // Fetch the bitmask for the full line relating to this render.
    std::vector<uint8_t> maskBuffer;
    maskBuffer.resize((pDraw->iWidth * sizeof(uint8_t)) / 8); // Each pixel has 1 bit of mask.
    instance->png.getAlphaMask(pDraw, maskBuffer.data(), 255);
    uint8_t* currentMask = maskBuffer.data();

    // TODO: mirroring via the lastImageData settings. 
    // Draw in byte chunks for the entire line, utilising the mask to know when a pixel is opaque.
    for (int x = 0; x < pDraw->iWidth; x+=8) {
        // For this iteration of a byte grab the current mask
        uint8_t mask = *currentMask;

        // Loop through the entire byte, querying each bit to see if its a opaque; (1=opaque, 0=transparent).
        for (int iBit = 0; iBit < 8; ++iBit) {
            // Check the leftmost bit / Most significant bit (MSB) and if set draw.
            if (!instance->lastImageData.useTransparency || mask & 0x80) {
                auto rgbVal = pimoroni::RGB565(*currentPixel);
                instance->context->set_pixel_dither(
                    pimoroni::Point{
                        instance->lastPositionData.x + x + iBit,
                        instance->lastPositionData.y + pDraw->y}, 
                    rgbVal);
            }

            // Left shift the bits so that the next bit we want to operate on in the loop is the MSB.
            mask <<=1;
            // Increment the pixel that we're working on.
            ++currentPixel;
        }
        // At the end of the byte, increment the mask by a byte to move onto the next set.
        ++currentMask;
    }
    
    // For some reason returning a PNG_SUCCESS (which is 0) causes the library to EARLY quit, 
    // lowkey dumb but changed to allow for processing of this func
    return 1;
}
