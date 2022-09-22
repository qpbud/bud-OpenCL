#include "common/Except.hpp"
#include "device/Device.hpp"
#include "device/backend/vulkan/Device.hpp"

namespace qp::cl {

template<>
Device::Device(Vulkan)
    : Object<_cl_device_id>()
    , m_detail() {
    m_detail = std::make_unique<backend::vulkan::Device>();
}

cl_device_type Device::type() const {
    return m_detail->type();
}

size_t Device::getInfoSize(cl_device_info info) const {
    return m_detail->getInfoSize(info);
}

void Device::getInfo(cl_device_info info, size_t size, void* value) const {
    m_detail->getInfo(info, size, value);
}

}
