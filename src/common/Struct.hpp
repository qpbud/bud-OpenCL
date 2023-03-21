#pragma once

#include <array>
#include "common/Khronos.hpp"

namespace bud::cl {

struct CopyRegion {
    struct {
        std::array<size_t, 3> origin;
        size_t rowPitch;
        size_t slicePitch;
    } src, dst;
    std::array<size_t, 3> region;
};

}
