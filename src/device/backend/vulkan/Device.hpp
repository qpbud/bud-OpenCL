#pragma once

#include "device/detail/Device.hpp"

namespace qp::cl::backend::vulkan {

class Device : public detail::Device {
public:
    Device();
};

}
