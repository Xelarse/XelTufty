#include "include/scenes/sonaScene.hpp"

#include "include/images/icon240x240.h"
#include "include/utilities.hpp"

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

    renderLayers.emplace_back(std::make_unique<BearPawBackgroundLayer>(gc)); 
    renderLayers.emplace_back(std::make_unique<ImageLayer>(
        gc, 
        reinterpret_cast<Xel::LayerData*>(&bearIconImageData),  
        const_cast<uint8_t*>(Icon240x240),
        sizeof(Icon240x240)
    )); 
    renderLayers.emplace_back(std::make_unique<TextLayer>(gc, reinterpret_cast<Xel::LayerData*>(&textData))); 
}

auto SonaScene::update(Xel::Event* eventChain) -> void
{
    auto currentEvent = eventChain;
    while(currentEvent) {
        switch(currentEvent->type) {
            case Xel::EventType::AMBIENT_LIGHT:
            // I know bad design but I know the order I put them in.
            // Also cant use dynamic_cast to downcast here because rtti is disabled.
            auto ambient = reinterpret_cast<Xel::AmbientLightEvent*>(currentEvent);
            auto bg = static_cast<BearPawBackgroundLayer*>(renderLayers.front().get());
            auto text = static_cast<TextLayer*>(renderLayers.back().get());

            // TODO: Create a hysterisis generic to manage bouncing of edge values.
            if (ambient->scale < 0.5f) {
                bg->darkMode(true);
                text->setColour(Xel::RGB_CONSTS::WHITE);
            }
            else {
                bg->darkMode(false);
                text->setColour(Xel::RGB_CONSTS::BLACK);
            }
            break;
        }
        currentEvent = currentEvent->next;
    }


    for (auto& layer : renderLayers) {
        layer->update();
    }
}