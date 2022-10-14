#pragma once

#include "device/detail/Context.hpp"
#include "device/detail/HostQueue.hpp"

namespace qp::cl::backend::vulkan {

class Context : public detail::Context {
public:
    std::unique_ptr<detail::HostQueue> createHostQueue() override;
};

}
