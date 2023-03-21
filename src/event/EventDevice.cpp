#include "event/EventDevice.hpp"

namespace bud::cl {

Event<EventBase::Type::device>::Event(Context& context, Device& device)
    : EventBase(context, EventBase::Type::device)
    , H1D1<hal::Event>(device, context[device]) {
}

void Event<EventBase::Type::device>::hostWait() {}

void Event<EventBase::Type::device>::hostSet() {}

}
