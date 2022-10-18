#pragma once

#include "device/detail/Device.hpp"

namespace qp::cl::backend::cpu {

class Device : public detail::Device {
public:
    Device();

    std::unique_ptr<detail::Context> createContext() override;
};

}
