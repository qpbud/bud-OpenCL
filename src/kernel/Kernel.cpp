#include "kernel/Kernel.hpp"

namespace bud::cl {

Kernel::Kernel(Context& context, Program& program, std::string&& entry)
    : Object<_cl_kernel>()
    , Scope(context)
    , H1DN<hal::Kernel>()
    , m_program(&program)
    , m_entry(std::move(entry)) {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

Kernel::Kernel(Context& context, const Kernel& srcKernel)
    : Object<_cl_kernel>()
    , Scope(context)
    , H1DN<hal::Kernel>()
    , m_program(srcKernel.m_program)
    , m_entry(srcKernel.m_entry) {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

void Kernel::setArg(cl_uint index, size_t size, const void* value) {}

void Kernel::setArgSvmPointer(cl_uint index, const void* value) {}

size_t Kernel::getInfoSize(cl_kernel_info info) const {
    switch (info) {
        case CL_KERNEL_FUNCTION_NAME:
            return m_entry.size() + 1;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Kernel::getInfo(cl_kernel_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_KERNEL_FUNCTION_NAME:
            m_entry.copy(static_cast<char*>(value), m_entry.size());
            static_cast<char*>(value)[m_entry.size()] = '\0';
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

}
