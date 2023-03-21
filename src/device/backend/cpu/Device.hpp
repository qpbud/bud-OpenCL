#pragma once

#include "device/hal/Device.hpp"

namespace bud::cl::backend::cpu {

class Device : public hal::Device {
public:
    Device();

    std::unique_ptr<hal::Context> createContext() override;
};

}
