#include "event/EventHost.hpp"

namespace qp::cl {

Event<EventBase::Type::host>::Event(Context& context)
    : EventBase(context, EventBase::Type::host) {
}

void Event<EventBase::Type::host>::setUserStatus(cl_int status) {}

void Event<EventBase::Type::host>::hostWait() {}

void Event<EventBase::Type::host>::hostSet() {}

}
