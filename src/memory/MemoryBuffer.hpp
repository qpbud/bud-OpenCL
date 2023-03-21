#pragma once

#include <vector>
#include "device/H2D.hpp"
#include "memory/MemoryBase.hpp"

namespace bud::cl {

template<>
class Memory<MemoryBase::Type::buffer> : public MemoryBase, public H1DN<hal::Buffer> {
    size_t m_size;
public:
    Memory(Context& context,
           std::vector<cl_mem_properties>&& properties,
           cl_mem_flags flags,
           void* hostPtr,
           size_t size);

    size_t size() const;
};

}
