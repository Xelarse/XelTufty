#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"

namespace Xel {
    struct LayerData {
        enum class Type {
            NONE,
            INPUT,
            IMAGE
        };

        virtual ~LayerData() = default;

        Type type = Type::NONE;
    };

    struct ImageData final : public LayerData {
        enum class MirrorFlags : uint8_t {
            NONE = 0,
            MIRROR_X = 1 << 0,
            MIRROR_Y = 1 << 1
        };

        ~ImageData() override = default;

        uint16_t xpos = 0;
        uint16_t ypos = 0;
        MirrorFlags mirrorFlags = MirrorFlags::NONE;
        bool hasTransparency = false;

        /*
            Flag usage
            MirrorFlags flags = MIRROR_X | MIRROR_Y;
            if (flags & MIRROR_X) {}
        */
    };

    struct InputData final : public LayerData {
        ~InputData() override = default;
        bool a = false;
        bool b = false;
        bool c = false;
        bool up = false;
        bool down = false;
    };

    class Layer {
        public:
            virtual ~Layer() = default;
            Layer(Layer& lhs) = delete;
            Layer(Layer&& lhs) = delete;
            Layer& operator=(Layer& rhs) = delete;
            Layer&& operator=(Layer&& rhs) = delete;

            // FIXME: C++20 doesnt seem supported so cant use span here
            virtual auto update(double dt = .0, const std::vector<LayerData*>& layerData = {}) -> void = 0;

        protected:
            Layer(pimoroni::PicoGraphics* gc) : context{gc} {};

            // FIXME: Should this be a shared ptr? Dont think we want heap on an embedded device so a raw ptr to the object should be fine.
            pimoroni::PicoGraphics* context = nullptr;

        private:
            Layer() = default;
    };
}