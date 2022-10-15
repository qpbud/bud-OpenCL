#pragma once

#include <vector>
#include "device/H2D.hpp"
#include "queue/QueueBase.hpp"

namespace qp::cl {

template<>
class Queue<QueueBase::Type::host> : public QueueBase, public H1D1<detail::HostQueue> {
public:
    Queue(Context& context,
          Device& device,
          std::vector<cl_queue_properties>&& properties,
          cl_command_queue_properties propertyBits,
          bool enableProfiling);
};

}
