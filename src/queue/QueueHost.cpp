#include "queue/QueueHost.hpp"

namespace bud::cl {

Queue<QueueBase::Type::host>::Queue(Context& context,
                                    Device& device,
                                    std::vector<cl_queue_properties>&& properties,
                                    cl_command_queue_properties propertyBits,
                                    bool enableProfiling)
    : QueueBase(context, QueueBase::Type::host, &device, std::move(properties), propertyBits)
    , H1D1<hal::HostQueue>(device, context[device])
    , m_enableProfiling(enableProfiling)
    , m_hostCommandList()
    , m_deviceCommandList() {
}

void Queue<QueueBase::Type::host>::flush() {}

void Queue<QueueBase::Type::host>::finish() {}

}
