#pragma once

#include "common/Struct.hpp"
#include "device/hal/Buffer.hpp"

namespace bud::cl::hal {

class Command {
public:
    virtual ~Command() = default;
    virtual void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, const CopyRegion& region) = 0;
};

}
