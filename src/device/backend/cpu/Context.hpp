#pragma once

#include "device/detail/Context.hpp"
#include "device/detail/HostQueue.hpp"
#include "device/detail/Buffer.hpp"

namespace qp::cl::backend::cpu {

class Context : public detail::Context {
public:
    std::unique_ptr<detail::HostQueue> createHostQueue() override;
    std::unique_ptr<detail::Buffer> createBuffer() override;
    std::unique_ptr<detail::Command> createCommand() override;
};

}
