#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"

namespace Xel {
    enum class EventType {
        NONE,
        BUTTON,
        AMBIENT_LIGHT,
        VOLTAGE
    };

    struct Event {
        Event(EventType t) : type{t}, next{nullptr} {}
        virtual ~Event() = default;

        EventType type = EventType::NONE;
        Event* next = nullptr;
    };

    struct ButtonEvent final : public Event {
        enum class Button {
            NONE,
            A,
            B,
            C,
            UP,
            DOWN
        };

        ButtonEvent() : Event{EventType::BUTTON} {};
        ~ButtonEvent() override = default;

        Button pressedButton = Button::NONE;
    };

    struct AmbientLightEvent final : public Event {
        AmbientLightEvent() : Event{EventType::AMBIENT_LIGHT} {};
        ~AmbientLightEvent() override = default;
        float scale;
    };

    struct VoltageEvent final : public Event {
        VoltageEvent() : Event{EventType::VOLTAGE} {};
        ~VoltageEvent() override = default;
        double voltage;
        float percentage;
        bool isUsbPowered;
    };

    class Scene {
        public:
            virtual ~Scene() = default;
            Scene(Scene& lhs) = delete;
            Scene(Scene&& lhs) = delete;
            Scene& operator=(Scene& rhs) = delete;
            Scene&& operator=(Scene&& rhs) = delete;

            virtual auto update(Event* eventChain = nullptr) -> void = 0;

        protected:
            Scene(pimoroni::PicoGraphics* gc) : context{gc} {};

            pimoroni::PicoGraphics* context = nullptr;

        private:
            Scene() = default;
    };
}