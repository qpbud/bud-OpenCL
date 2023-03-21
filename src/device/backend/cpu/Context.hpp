#pragma once

#include "device/hal/Context.hpp"

namespace bud::cl::backend::cpu {

class Context : public hal::Context {
public:
    std::unique_ptr<hal::HostQueue> createHostQueue() override;
    std::unique_ptr<hal::Buffer> createBuffer(bool isImport, void* hostPtr, size_t size) override;
    std::unique_ptr<hal::Command> createCommand() override;
    std::unique_ptr<hal::Event> createEvent() override;
    std::unique_ptr<hal::Image> createImage() override;
    std::unique_ptr<hal::Pipe> createPipe() override;
    std::unique_ptr<hal::Sampler> createSampler() override;
    std::unique_ptr<hal::Program> createProgram() override;
    std::unique_ptr<hal::Kernel> createKernel() override;
};

}
