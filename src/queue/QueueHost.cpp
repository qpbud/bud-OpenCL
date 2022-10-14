#include "queue/QueueHost.hpp"

namespace qp::cl {

Queue<QueueBase::Type::host>::Queue(Context& context, Device& device, std::vector<cl_queue_properties>&& properties, bool enableProfiling)
    : QueueBase(context, QueueBase::Type::host)
    , H1D1<detail::HostQueue>(device, context[device]) {
}

}
