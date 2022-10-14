#include <cstring>
#include "context/Context.hpp"

namespace qp::cl {

Context& Context::construct(std::vector<cl_context_properties>&& properties,
                            std::vector<cl_device_id>&& devices,
                            std::function<void(const char*, const void*, size_t)>&& notify) {
    auto context = new Context(std::move(properties), std::move(devices), std::move(notify));
    return *context;
}

Context::Context(std::vector<cl_context_properties>&& properties,
                 std::vector<cl_device_id>&& devices,
                 std::function<void(const char*, const void*, size_t)>&& notify)
    : Object<_cl_context>()
    , H1DN<detail::Context>()
    , m_properties(std::move(properties))
    , m_devices(std::move(devices))
    , m_notify(std::move(notify))
    , m_destructorCallbacks() {
    for (auto device : m_devices) {
        auto& deviceInternal = static_cast<Device&>(*device);
        append(deviceInternal);
    }
}

Context::~Context() {
    while (!m_destructorCallbacks.empty()) {
        m_destructorCallbacks.top()();
        m_destructorCallbacks.pop();
    }
}

size_t Context::getInfoSize(cl_context_info info) const {
    switch (info) {
        case CL_CONTEXT_REFERENCE_COUNT:
            return sizeof(cl_uint);
        case CL_CONTEXT_NUM_DEVICES:
            return sizeof(cl_uint);
        case CL_CONTEXT_DEVICES:
            return sizeof(cl_device_id) * m_devices.size();
        case CL_CONTEXT_PROPERTIES:
            return sizeof(cl_context_properties) * m_properties.size();
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Context::getInfo(cl_context_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_CONTEXT_REFERENCE_COUNT:
            *static_cast<cl_uint*>(value) = count();
            break;
        case CL_CONTEXT_NUM_DEVICES:
            *static_cast<cl_uint*>(value) = static_cast<cl_uint>(m_devices.size());
            break;
        case CL_CONTEXT_DEVICES:
            std::memcpy(value, m_devices.data(), sizeof(cl_device_id) * m_devices.size());
            break;
        case CL_CONTEXT_PROPERTIES:
            std::memcpy(value, m_properties.data(), sizeof(cl_context_properties) * m_properties.size());
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Context::setDestructorCallback(std::function<void()>&& destructorCallback) {
    m_destructorCallbacks.push(std::move(destructorCallback));
}

}
