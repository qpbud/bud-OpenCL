#include "event/EventBase.hpp"

namespace qp::cl {

EventBase::EventBase(Context& context,
                     EventBase::Type type)
    : Object<_cl_event>()
    , Scope(context)
    , m_type(type) {}

size_t EventBase::getInfoSize(cl_event_info info) const {
    switch (info) {
        case CL_EVENT_COMMAND_QUEUE:
            return sizeof(cl_command_queue);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void EventBase::getInfo(cl_event_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_EVENT_COMMAND_QUEUE:
            *static_cast<cl_command_queue*>(value) = nullptr;
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

EventBase::Type EventBase::type() const {
    return m_type;
}

}
