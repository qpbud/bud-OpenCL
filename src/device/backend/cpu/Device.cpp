#include "common/Khronos.hpp"
#include "device/backend/cpu/Device.hpp"
#include "device/backend/cpu/Context.hpp"

namespace bud::cl::backend::cpu {

Device::Device()
    : hal::Device(CL_DEVICE_TYPE_CPU) {
}

std::unique_ptr<hal::Context> Device::createContext() {
    return std::make_unique<Context>();
}

}