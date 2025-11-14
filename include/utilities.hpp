#pragma once

namespace Xel {
    struct RGB {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    namespace RGB_CONSTS {
        static constexpr RGB BEAR_BASE{254, 246, 210};
        static constexpr RGB BEAR_FLESH{246, 216, 190};
        static constexpr RGB BEAR_NOSE{186, 145, 99};
        static constexpr RGB BEAR_DECAL_LIGHT{252, 224, 138};
        static constexpr RGB BEAR_DECAL_DARK{232, 188, 141};
        static constexpr RGB BLACK{0, 0, 0};
        static constexpr RGB WHITE{255,255,255};
    }
}


