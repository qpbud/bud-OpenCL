#pragma once

#include <vector>
#include "device/H2D.hpp"
#include "memory/MemoryBase.hpp"

namespace bud::cl {

template<>
class Memory<MemoryBase::Type::pipe> : public MemoryBase, public H1DN<hal::Pipe> {
public:
    Memory(Context& context,
           cl_mem_flags flags);
};

}
