#include "device/backend/vulkan/Context.hpp"
#include "device/backend/vulkan/HostQueue.hpp"
#include "device/backend/vulkan/Buffer.hpp"
#include "device/backend/vulkan/Command.hpp"

namespace qp::cl::backend::vulkan {

std::unique_ptr<detail::HostQueue> Context::createHostQueue() {
    return std::make_unique<HostQueue>();
}

std::unique_ptr<detail::Buffer> Context::createBuffer() {
    return std::make_unique<Buffer>();
}

std::unique_ptr<detail::Command> Context::createCommand() {
    return std::make_unique<Command>();
}

}
