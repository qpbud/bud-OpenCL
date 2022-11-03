#include "memory/MemoryBuffer.hpp"

namespace qp::cl {

Memory<MemoryBase::Type::buffer>::Memory(Context& context,
                                         std::vector<cl_mem_properties>&& properties,
                                         cl_mem_flags flags,
                                         void* hostPtr,
                                         size_t size)
    : MemoryBase(context, MemoryBase::Type::buffer, CL_MEM_OBJECT_BUFFER, std::move(properties), flags, hostPtr)
    , H1DN<detail::Buffer>()
    , m_size(size) {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

size_t Memory<MemoryBase::Type::buffer>::size() const {
    return m_size;
}

}
