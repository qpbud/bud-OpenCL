#include "queue/QueueBase.hpp"

namespace qp::cl {

QueueBase::QueueBase(Context& context, QueueBase::Type type)
    : Object<_cl_command_queue>()
    , Scope(context)
    , m_type(type) {}

}
