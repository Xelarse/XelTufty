#include "include/scenes/sonaScene.hpp"

#include "include/images/icon240x240.h"

SonaScene::SonaScene(pimoroni::PicoGraphics* gc) : Xel::Scene{gc}
{
    // Icon positioning and render options
    Xel::PositionData bearIconPosition{};
    bearIconPosition.x = 0;
    bearIconPosition.y = 0;

    Xel::ImageData bearIconImageData{};
    bearIconImageData.mirrorFlags = Xel::ImageData::MirrorFlags::MIRROR_X | Xel::ImageData::MirrorFlags::MIRROR_Y;
    bearIconImageData.next = reinterpret_cast<Xel::LayerData*>(&bearIconPosition);

    // Text positioning and render options
    Xel::PositionData textPosition{};
    textPosition.x = 310;
    textPosition.y = 230;

    Xel::TextData textData{};
    textData.next = reinterpret_cast<Xel::LayerData*>(&textPosition);
    textData.text = "Xel";
    textData.scale = 6.8f;
    textData.rotation = 180.f;


    renderLayers.push_back(BearPawBackgroundLayer{gc});
    renderLayers.push_back(ImageLayer{
        gc, 
        reinterpret_cast<Xel::LayerData*>(&bearIconImageData), 
        const_cast<uint8_t*>(Icon240x240), 
        sizeof(Icon240x240)}
    );
    renderLayers.push_back(TextLayer{gc, reinterpret_cast<Xel::LayerData*>(&textData)});
}

auto SonaScene::update(Xel::Event* eventChain) -> void
{
    // std::visit checks all types at compile time to see if the lambda is valid against each
    // type stored in the possible variants, no v-table lookup for derived classes and no runtime hit. Pretty cool.
    for (auto& layer : renderLayers) {
        std::visit([](auto& l){ l.update(); }, layer);
    }

}