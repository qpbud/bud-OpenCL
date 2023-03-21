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

namespace bud::cl::backend::vulkan {

std::unique_ptr<hal::HostQueue> Context::createHostQueue() {
    return std::make_unique<HostQueue>();
}

std::unique_ptr<hal::Buffer> Context::createBuffer(bool isImport, void* hostPtr, size_t size) {
    return std::make_unique<Buffer>(isImport, hostPtr, size);
}

std::unique_ptr<hal::Command> Context::createCommand() {
    return std::make_unique<Command>();
}

std::unique_ptr<hal::Event> Context::createEvent() {
    return std::make_unique<Event>();
}

std::unique_ptr<hal::Image> Context::createImage() {
    return std::make_unique<Image>();
}

std::unique_ptr<hal::Pipe> Context::createPipe() {
    return std::make_unique<Pipe>();
}

std::unique_ptr<hal::Sampler> Context::createSampler() {
    return std::make_unique<Sampler>();
}

std::unique_ptr<hal::Program> Context::createProgram() {
    return std::make_unique<Program>();
}

std::unique_ptr<hal::Kernel> Context::createKernel() {
    return std::make_unique<Kernel>();
}

}
