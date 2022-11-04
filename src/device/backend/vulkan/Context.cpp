#include "device/backend/vulkan/Context.hpp"
#include "device/backend/vulkan/HostQueue.hpp"
#include "device/backend/vulkan/Buffer.hpp"
#include "device/backend/vulkan/Command.hpp"
#include "device/backend/vulkan/Event.hpp"
#include "device/backend/vulkan/Image.hpp"
#include "device/backend/vulkan/Pipe.hpp"
#include "device/backend/vulkan/Sampler.hpp"
#include "device/backend/vulkan/Program.hpp"
#include "device/backend/vulkan/Kernel.hpp"

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

std::unique_ptr<detail::Event> Context::createEvent() {
    return std::make_unique<Event>();
}

std::unique_ptr<detail::Image> Context::createImage() {
    return std::make_unique<Image>();
}

std::unique_ptr<detail::Pipe> Context::createPipe() {
    return std::make_unique<Pipe>();
}

std::unique_ptr<detail::Sampler> Context::createSampler() {
    return std::make_unique<Sampler>();
}

std::unique_ptr<detail::Program> Context::createProgram() {
    return std::make_unique<Program>();
}

std::unique_ptr<detail::Kernel> Context::createKernel() {
    return std::make_unique<Kernel>();
}

}
