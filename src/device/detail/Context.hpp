#pragma once

#include <memory>
#include "device/detail/HostQueue.hpp"

namespace qp::cl::detail {

class Context {
public:
    virtual ~Context() = default;

    virtual std::unique_ptr<HostQueue> createHostQueue() = 0;
};

}
