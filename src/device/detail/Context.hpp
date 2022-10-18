#pragma once

#include <memory>
#include "device/detail/HostQueue.hpp"
#include "device/detail/Buffer.hpp"
#include "device/detail/Command.hpp"

namespace qp::cl::detail {

class Context {
public:
    virtual ~Context() = default;

    virtual std::unique_ptr<HostQueue> createHostQueue() = 0;
    virtual std::unique_ptr<Buffer> createBuffer() = 0;
    virtual std::unique_ptr<Command> createCommand() = 0;
};

}
