#include "event/EventDevice.hpp"

namespace qp::cl {

Event<EventBase::Type::device>::Event(Context& context, Device& device)
    : EventBase(context, EventBase::Type::device)
    , H1D1<detail::Event>(device, context[device]) {
}

void Event<EventBase::Type::device>::hostWait() {}

void Event<EventBase::Type::device>::hostSet() {}

}
