#pragma once

#include "device/hal/Command.hpp"
#include "common/Chain.hpp"

namespace bud::cl::backend::cpu {

class Command : public hal::Command {
    Chain<cl_int(cl_int)> m_chain;
public:
    void copyBuffer(hal::Buffer& srcBuffer, hal::Buffer& dstBuffer, const CopyRegion& region) override;
};

}
