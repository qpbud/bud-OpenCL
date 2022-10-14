#include "device/backend/vulkan/Context.hpp"
#include "device/backend/vulkan/HostQueue.hpp"

namespace qp::cl::backend::vulkan {

std::unique_ptr<detail::HostQueue> Context::createHostQueue() {
    return std::make_unique<HostQueue>();
}

}
