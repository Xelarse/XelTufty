#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"

namespace Xel {
    enum class EventType {
        NONE,
        BUTTON,
        PHOTO_RESISTOR,
        VOLTAGE
    };

    struct Event {
        Event(EventType t) : type{t}, next{nullptr} {}
        virtual ~Event() = default;

        EventType type = EventType::NONE;
        Event* next = nullptr;
    };

    struct ButtonEvent final : public Event {
        ButtonEvent() : Event{EventType::BUTTON} {};
        ~ButtonEvent() override = default;
    };

    struct LightEvent final : public Event {
        LightEvent() : Event{EventType::PHOTO_RESISTOR} {};
        ~LightEvent() override = default;
    };

    struct VoltageEvent final : public Event {
        VoltageEvent() : Event{EventType::VOLTAGE} {};
        ~VoltageEvent() override = default;
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