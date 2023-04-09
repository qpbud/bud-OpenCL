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

namespace bud::cl::backend::cpu {

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

std::unique_ptr<hal::Program> Context::createProgram(hal::Program::SourceVariant&& sourceVariant) {
    return std::make_unique<Program>(std::move(sourceVariant));
}

std::unique_ptr<hal::Kernel> Context::createKernel() {
    return std::make_unique<Kernel>();
}

}
