#include "device/backend/cpu/Context.hpp"
#include "device/backend/cpu/HostQueue.hpp"
#include "device/backend/cpu/Buffer.hpp"
#include "device/backend/cpu/Command.hpp"

namespace qp::cl::backend::cpu {

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
