#pragma once

#include "device/detail/Context.hpp"

namespace qp::cl::backend::vulkan {

class Context : public detail::Context {
public:
    std::unique_ptr<detail::HostQueue> createHostQueue() override;
    std::unique_ptr<detail::Buffer> createBuffer() override;
    std::unique_ptr<detail::Command> createCommand() override;
    std::unique_ptr<detail::Event> createEvent() override;
    std::unique_ptr<detail::Image> createImage() override;
};

}
