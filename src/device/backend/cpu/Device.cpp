#include "common/Khronos.hpp"
#include "device/backend/cpu/Device.hpp"
#include "device/backend/cpu/Context.hpp"

namespace qp::cl::backend::cpu {

Device::Device()
    : detail::Device(CL_DEVICE_TYPE_CPU) {
}

std::unique_ptr<detail::Context> Device::createContext() {
    return std::make_unique<Context>();
}

}
