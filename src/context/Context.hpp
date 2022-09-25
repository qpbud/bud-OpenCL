#pragma once

#include <functional>
#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "device/H2D.hpp"

namespace qp::cl {

class Context : public Object<_cl_context>, public H1DN<detail::Context> {
    std::vector<cl_context_properties> m_properties;
    std::vector<cl_device_id> m_devices;
    std::function<void(const char*, const void*, size_t, void*)> m_notify;
    void* m_userData;
public:
    static Context& create(std::vector<cl_context_properties>&& properties,
                           std::vector<cl_device_id>&& devices,
                           std::function<void(const char*, const void*, size_t, void*)> notify,
                           void* userData);

    Context(std::vector<cl_context_properties>&& properties,
            std::vector<cl_device_id>&& devices,
            std::function<void(const char*, const void*, size_t, void*)> notify,
            void* userData);
};

}
