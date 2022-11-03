#pragma once

#include <vector>
#include "device/H2D.hpp"
#include "memory/MemoryBase.hpp"

namespace qp::cl {

template<>
class Memory<MemoryBase::Type::pipe> : public MemoryBase, public H1DN<detail::Pipe> {
public:
    Memory(Context& context,
           cl_mem_flags flags);
};

}
