#include "common/Except.hpp"
#include "device/Device.hpp"
#include "device/backend/vulkan/Device.hpp"

namespace qp::cl {

Device::Device(RegisteredDevices registeredDevice)
    : Object<_cl_device_id>()
    , m_type(CL_DEVICE_TYPE_DEFAULT)
    , m_detail() {
    switch (registeredDevice) {
        case RegisteredDevices::vulkan:
            m_type = CL_DEVICE_TYPE_GPU;
            m_detail = std::make_unique<vk::Device>();
            break;
        default:
            throw Except(CL_OUT_OF_HOST_MEMORY);
    }
}

cl_device_type Device::type() const {
    return m_type;
}

}
