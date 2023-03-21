#include "program/Program.hpp"

namespace bud::cl {

Program::Program(Context& context,
                 std::vector<std::string>&& sources)
    : Object<_cl_program>()
    , Scope(context)
    , H1DN<hal::Program>()
    , m_sources(std::move(sources))
    , m_il()
    , m_devices()
    , m_binaries()
    , m_releaseCallbacks() {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

Program::Program(Context& context,
                 std::vector<unsigned char>&& il)
    : Object<_cl_program>()
    , Scope(context)
    , H1DN<hal::Program>()
    , m_sources()
    , m_il(std::move(il))
    , m_devices()
    , m_binaries()
    , m_releaseCallbacks() {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

Program::~Program() {
    while (!m_releaseCallbacks.empty()) {
        m_releaseCallbacks.top()();
        m_releaseCallbacks.pop();
    }
}

Program::Program(Context& context,
                 std::vector<cl_device_id>&& devices,
                 std::vector<std::vector<unsigned char>>&& binaries)
    : Object<_cl_program>()
    , Scope(context)
    , H1DN<hal::Program>()
    , m_sources()
    , m_il()
    , m_devices(std::move(devices))
    , m_binaries(std::move(binaries))
    , m_releaseCallbacks() {
    for (auto device : m_devices) {
        auto& deviceInternal = static_cast<Device&>(*device);
        append(deviceInternal, context[deviceInternal]);
    }
}

size_t Program::getInfoSize(cl_program_info info) const {
    switch (info) {
        case CL_PROGRAM_REFERENCE_COUNT:
            return sizeof(cl_uint);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Program::getInfo(cl_program_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_PROGRAM_REFERENCE_COUNT:
            *static_cast<cl_uint*>(value) = count();
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Program::setReleaseCallback(std::function<void()>&& releaseCallback) {
    m_releaseCallbacks.push(std::move(releaseCallback));
}

}
