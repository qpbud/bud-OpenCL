#pragma once

#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "event/EventBase.hpp"

namespace qp::cl {

template<>
class Event<EventBase::Type::device> : public EventBase, public H1D1<detail::Event> {
public:
    Event(Context& context, Device& device);

    void hostWait() override;
    void hostSet() override;
};

}
