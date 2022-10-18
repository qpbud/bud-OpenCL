#include "memory/MemoryBuffer.hpp"

namespace qp::cl {

Memory<MemoryBase::Type::buffer>::Memory(Context& context,
                                         std::vector<cl_mem_properties>&& properties,
                                         cl_mem_flags flags,
                                         void* hostPtr,
                                         size_t size)
    : MemoryBase(context, MemoryBase::Type::buffer, std::move(properties), flags, hostPtr)
    , H1DN<detail::Buffer>() {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

}
