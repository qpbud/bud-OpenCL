#pragma once

#include "device/hal/Command.hpp"

namespace bud::cl::backend::vulkan {

class Command : public hal::Command {
public:
    void copyBuffer(hal::Buffer& srcBuffer, hal::Buffer& dstBuffer, const CopyRegion& region) override {}
};

}
