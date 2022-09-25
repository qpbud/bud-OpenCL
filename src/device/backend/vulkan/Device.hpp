#pragma once

#include <memory>
#include "device/detail/Device.hpp"

namespace qp::cl::backend::vulkan {

class Device : public detail::Device {
public:
    Device();

    std::unique_ptr<detail::Context> createContext() override;
};

}
