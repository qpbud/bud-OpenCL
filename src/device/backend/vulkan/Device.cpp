#include "common/Khronos.hpp"
#include "device/backend/vulkan/Device.hpp"
#include "device/backend/vulkan/Context.hpp"

namespace bud::cl::backend::vulkan {

Device::Device()
    : hal::Device(CL_DEVICE_TYPE_GPU) {
}

std::unique_ptr<hal::Context> Device::createContext() {
    return std::make_unique<Context>();
}

}
