#pragma once

#include <memory>
#include "device/hal/HostQueue.hpp"
#include "device/hal/Buffer.hpp"
#include "device/hal/Command.hpp"
#include "device/hal/Event.hpp"
#include "device/hal/Image.hpp"
#include "device/hal/Pipe.hpp"
#include "device/hal/Sampler.hpp"
#include "device/hal/Program.hpp"
#include "device/hal/Kernel.hpp"

namespace bud::cl::hal {

class Context {
public:
    virtual ~Context() = default;

    virtual std::unique_ptr<HostQueue> createHostQueue() = 0;
    virtual std::unique_ptr<Buffer> createBuffer(bool isImport, void* hostPtr, size_t size) = 0;
    virtual std::unique_ptr<Command> createCommand() = 0;
    virtual std::unique_ptr<Event> createEvent() = 0;
    virtual std::unique_ptr<Image> createImage() = 0;
    virtual std::unique_ptr<Pipe> createPipe() = 0;
    virtual std::unique_ptr<Sampler> createSampler() = 0;
    virtual std::unique_ptr<Program> createProgram(Program::SourceVariant&& sourceVariant) = 0;
    virtual std::unique_ptr<Kernel> createKernel() = 0;
};

}
