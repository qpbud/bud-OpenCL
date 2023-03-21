#include "event/EventHost.hpp"

namespace bud::cl {

Event<EventBase::Type::host>::Event(Context& context)
    : EventBase(context, EventBase::Type::host) {
}

void Event<EventBase::Type::host>::setUserStatus(cl_int status) {}

void Event<EventBase::Type::host>::hostWait() {}

void Event<EventBase::Type::host>::hostSet() {}

}
