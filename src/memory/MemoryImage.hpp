#pragma once

#include <vector>
#include "device/H2D.hpp"
#include "memory/MemoryBase.hpp"

namespace bud::cl {

template<>
class Memory<MemoryBase::Type::image> : public MemoryBase, public H1DN<hal::Image> {
public:
    Memory(Context& context,
           std::vector<cl_mem_properties>&& properties,
           cl_mem_flags flags,
           void* hostPtr);
};

}
