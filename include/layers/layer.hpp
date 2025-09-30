#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"

namespace Xel {

    enum class LayerDataType {
        NONE,
        INPUT,
        IMAGE
    };

    // Layer data make use of struct chaining, allowing the receiving area if desired to have multiple 
    // packs of unrelated data for a given layer call
    // TODO: Make a helper that takes a LayerData::Type and traverses the struct chain to either give the resulting ptr or nullptr
    struct LayerData {
        virtual ~LayerData() = default;

        LayerDataType type = LayerDataType::NONE;
        LayerData* next = nullptr;
    };

    // TODO: break this out into PositionData and ImageData as in future text will also use position
    // ALSO make time data to get rid of dt calc
    struct ImageData final : public LayerData {
        enum class MirrorFlags : uint8_t {
            NONE = 0,
            MIRROR_X = 1 << 0,
            MIRROR_Y = 1 << 1
        };

        ~ImageData() override = default;

        uint16_t x = 0;
        uint16_t y = 0;
        MirrorFlags mirrorFlags = MirrorFlags::NONE;
        bool hasTransparency = false;
        bool drawBoarder = false;

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

            virtual auto update(LayerData* layerData = nullptr) -> void = 0;

        protected:
            Layer(pimoroni::PicoGraphics* gc) : context{gc} {};

            // FIXME: Should this be a shared ptr? Dont think we want heap on an embedded device so a raw ptr to the object should be fine.
            pimoroni::PicoGraphics* context = nullptr;

        private:
            Layer() = default;
    };
}