#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "include/utilities.hpp"

namespace Xel {

    enum class LayerDataType {
        NONE,
        POSITION,
        IMAGE,
        INPUT,
        TEXT
    };

    // Layer data make use of struct chaining, allowing the receiving area if desired to have multiple 
    // packs of unrelated data for a given layer call
    // TODO: Make a helper that takes a LayerData::Type and traverses the struct chain to either give the resulting ptr or nullptr
    struct LayerData {
        LayerData(LayerDataType t) : type{t}, next{nullptr} {}
        virtual ~LayerData() = default;

        LayerDataType type = LayerDataType::NONE;
        LayerData* next = nullptr;
    };

    struct PositionData final : public LayerData {
        PositionData() : LayerData{LayerDataType::POSITION} {}
        ~PositionData() override = default;

        uint16_t x = 0;
        uint16_t y = 0;
    };

    struct ImageData final : public LayerData {
        struct MirrorFlags {
            using Type = uint8_t;
            static constexpr Type NONE = 0;
            static constexpr Type MIRROR_X = 1 << 0;
            static constexpr Type MIRROR_Y = 1 << 1;
            static constexpr Type MAX = 1 << 7;
        };

        ImageData() : LayerData{LayerDataType::IMAGE} {}
        ~ImageData() override = default;

        MirrorFlags::Type mirrorFlags = MirrorFlags::NONE;
        bool useTransparency = true;
    };

    struct InputData final : public LayerData {
        InputData() : LayerData{LayerDataType::INPUT} {}
        ~InputData() override = default;
        bool a = false;
        bool b = false;
        bool c = false;
        bool up = false;
        bool down = false;
    };

    // TODO: Maybe break out scale and rotation to do a cheaper per frame update
    struct TextData final : public LayerData {
        TextData() : LayerData{LayerDataType::TEXT} {}        
        ~TextData() override = default;
        const char* text;
        const char* font = "bitmap8";
        int wrapPixelCount = 0;
        float scale = 1.0f;
        float rotation = 0.f;
        uint8_t spacing = 1;
        Xel::RGB col = {0,0,0};
    };

    class Layer {
        public:
            virtual ~Layer() = default;
            Layer(const Layer& lhs) = default;
            Layer& operator=(const Layer& rhs) = default;
            // Shallow copy on a move since ptr isn't owned
            Layer(Layer&& lhs) : context{lhs.context} {};
            Layer& operator=(Layer&& rhs) { context = rhs.context; return *this; };

            virtual auto update(LayerData* layerData = nullptr) -> void = 0;

        protected:
            Layer(pimoroni::PicoGraphics* gc) : context{gc} {};

            // FIXME: Should this be a shared ptr? Dont think we want heap on an embedded device so a raw ptr to the object should be fine.
            // FIXME: Should really make a graphics context abstraction for this so we arent hard coupled to PicoGraphics.
            pimoroni::PicoGraphics* context = nullptr;

        private:
            Layer() = default;
    };
}