#include "memory/MemoryPipe.hpp"

namespace bud::cl {

Memory<MemoryBase::Type::pipe>::Memory(Context& context,
                                       cl_mem_flags flags)
    : MemoryBase(context, MemoryBase::Type::pipe, CL_MEM_OBJECT_PIPE, {}, flags, nullptr)
    , H1DN<hal::Pipe>() {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

}
