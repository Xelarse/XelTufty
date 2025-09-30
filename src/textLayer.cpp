#include "include/layers/textLayer.hpp"

TextLayer::TextLayer(pimoroni::PicoGraphics* gc, Xel::LayerData* layerData) : Layer{gc}
{
    processDataChain(layerData);
}

auto TextLayer::update(Xel::LayerData* layerData) -> void
{
    if (layerData) { processDataChain(layerData); }
    context->set_pen(lastTextData.col.r, lastTextData.col.g, lastTextData.col.b);
    context->set_font(lastTextData.font);
    context->text(
        lastTextData.text, 
        {lastPositionData.x, lastPositionData.y}, 
        lastTextData.wrapPixelCount, 
        lastTextData.scale, 
        lastTextData.rotation, 
        lastTextData.spacing
    );
}

auto TextLayer::processDataChain(Xel::LayerData* layerData) -> void
{
    auto currentLayer = layerData;
    while(currentLayer) {
        switch(currentLayer->type) {
            case Xel::LayerDataType::TEXT: {
                lastTextData = *(reinterpret_cast<Xel::TextData*>(currentLayer));
                break;
            }
            case Xel::LayerDataType::POSITION: {
                lastPositionData = *(reinterpret_cast<Xel::PositionData*>(currentLayer));
                break;
            }
        }

        currentLayer = currentLayer->next;
    }    
}
