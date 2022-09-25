#include "context/Context.hpp"

namespace qp::cl {

Context& Context::create(std::vector<cl_context_properties>&& properties,
                         std::vector<cl_device_id>&& devices,
                         std::function<void(const char*, const void*, size_t, void*)> notify,
                         void* userData) {
    auto context = new Context(std::move(properties), std::move(devices), notify, userData);
    return *context;
}

Context::Context(std::vector<cl_context_properties>&& properties,
                 std::vector<cl_device_id>&& devices,
                 std::function<void(const char*, const void*, size_t, void*)> notify,
                 void* userData)
    : Object<_cl_context>()
    , H1DN<detail::Context>()
    , m_properties(std::move(properties))
    , m_devices(std::move(devices))
    , m_notify(std::move(notify))
    , m_userData(userData) {
    for (auto device : m_devices) {
        auto& deviceInternal = static_cast<Device&>(*device);
        append(deviceInternal);
    }
}

}
