#include "common/Khronos.hpp"
#include "device/backend/vulkan/Device.hpp"

namespace qp::cl::backend::vulkan {

Device::Device()
    : detail::Device(CL_DEVICE_TYPE_GPU) {
}

}
