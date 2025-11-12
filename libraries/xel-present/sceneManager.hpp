#pragma once

#include "scene.hpp"

namespace xel {
    class SceneManager final {
        public:
            SceneManager(pimoroni::PicoGraphics* context) : context{context} {};
            ~SceneManager() = default;

            auto update(Xel::Event* eventChain) -> void;

        private:
            SceneManager() = delete;
            pimoroni::PicoGraphics* context = nullptr;
    };
}