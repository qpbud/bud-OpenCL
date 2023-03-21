#include "common/Except.hpp"
#include "device/Device.hpp"
#include "device/backend/vulkan/Device.hpp"
#include "device/backend/cpu/Device.hpp"

namespace bud::cl {

template<>
Device::Device(Cpu)
    : Object<_cl_device_id>()
    , m_hal() {
    m_hal = std::make_unique<backend::cpu::Device>();
}

template<>
Device::Device(Vulkan)
    : Object<_cl_device_id>()
    , m_hal() {
    m_hal = std::make_unique<backend::vulkan::Device>();
}

cl_device_type Device::type() const {
    return m_hal->type();
}

size_t Device::getInfoSize(cl_device_info info) const {
    return m_hal->getInfoSize(info);
}

void Device::getInfo(cl_device_info info, size_t size, void* value) const {
    m_hal->getInfo(info, size, value);
}

Device::operator hal::Device&() { return *m_hal; }

}
