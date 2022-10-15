#include "queue/QueueHost.hpp"

namespace qp::cl {

Queue<QueueBase::Type::host>::Queue(Context& context,
                                    Device& device,
                                    std::vector<cl_queue_properties>&& properties,
                                    cl_command_queue_properties propertyBits,
                                    bool enableProfiling)
    : QueueBase(context, QueueBase::Type::host, &device, std::move(properties), propertyBits)
    , H1D1<detail::HostQueue>(device, context[device]) {
}

}
