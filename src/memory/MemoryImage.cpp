#include "memory/MemoryImage.hpp"

namespace qp::cl {

Memory<MemoryBase::Type::image>::Memory(Context& context,
                                        std::vector<cl_mem_properties>&& properties,
                                        cl_mem_flags flags,
                                        void* hostPtr)
    : MemoryBase(context, MemoryBase::Type::image, CL_MEM_OBJECT_IMAGE1D, std::move(properties), flags, hostPtr)
    , H1DN<detail::Image>() {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

}
