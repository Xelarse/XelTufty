#pragma once

#include "scene.hpp"

namespace xel {
    class SceneManager final {
        public:
            SceneManager() = default;
            ~SceneManager() = default;

            auto update(Xel::Event* eventChain) -> void;
    };
}