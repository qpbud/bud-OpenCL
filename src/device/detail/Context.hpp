#pragma once

#include <memory>
#include "device/detail/HostQueue.hpp"
#include "device/detail/Buffer.hpp"
#include "device/detail/Command.hpp"
#include "device/detail/Event.hpp"
#include "device/detail/Image.hpp"
#include "device/detail/Pipe.hpp"
#include "device/detail/Sampler.hpp"
#include "device/detail/Program.hpp"
#include "device/detail/Kernel.hpp"

namespace qp::cl::detail {

class Context {
public:
    virtual ~Context() = default;

    virtual std::unique_ptr<HostQueue> createHostQueue() = 0;
    virtual std::unique_ptr<Buffer> createBuffer() = 0;
    virtual std::unique_ptr<Command> createCommand() = 0;
    virtual std::unique_ptr<Event> createEvent() = 0;
    virtual std::unique_ptr<Image> createImage() = 0;
    virtual std::unique_ptr<Pipe> createPipe() = 0;
    virtual std::unique_ptr<Sampler> createSampler() = 0;
    virtual std::unique_ptr<Program> createProgram() = 0;
    virtual std::unique_ptr<Kernel> createKernel() = 0;
};

}
