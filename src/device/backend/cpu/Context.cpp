#include "device/backend/cpu/Context.hpp"
#include "device/backend/cpu/HostQueue.hpp"
#include "device/backend/cpu/Buffer.hpp"
#include "device/backend/cpu/Command.hpp"
#include "device/backend/cpu/Event.hpp"
#include "device/backend/cpu/Image.hpp"
#include "device/backend/cpu/Pipe.hpp"
#include "device/backend/cpu/Sampler.hpp"
#include "device/backend/cpu/Program.hpp"
#include "device/backend/cpu/Kernel.hpp"

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
