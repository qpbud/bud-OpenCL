#include "common/Khronos.hpp"
#include "device/backend/vulkan/Device.hpp"
#include "device/backend/vulkan/Context.hpp"

namespace qp::cl::backend::vulkan {

Device::Device()
    : detail::Device(CL_DEVICE_TYPE_GPU) {
}

std::unique_ptr<detail::Context> Device::createContext() {
    return std::make_unique<Context>();
}

}
