#pragma once

#include "device/hal/Device.hpp"

namespace bud::cl::backend::vulkan {

class Device : public hal::Device {
public:
    Device();

    std::unique_ptr<hal::Context> createContext() override;
};

}
