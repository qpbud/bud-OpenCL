#pragma once

#include <memory>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/detail/Device.hpp"

namespace qp::cl {

class Device : public Object<_cl_device_id> {
    cl_device_type m_type;
    std::unique_ptr<detail::Device> m_detail;
public:
    enum class RegisteredDevices {
        vulkan
    };

    Device(RegisteredDevices registeredDevice);
    cl_device_type type() const;
};

}
