#pragma once

#include "event/EventBase.hpp"

namespace qp::cl {

template<>
class Event<EventBase::Type::host> : public EventBase {
public:
    Event(Context& context);

    void setUserStatus(cl_int status);

    void hostWait() override;
    void hostSet() override;
};

}
