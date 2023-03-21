#pragma once

#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace bud::cl {

class EventBase : public Object<_cl_event>, public Scope {
public:
    enum class Type { host, device };
private:
    Type m_type;
public:
    EventBase(Context& context,
              Type type);
    virtual ~EventBase() = default;

    size_t getInfoSize(cl_event_info info) const;
    void getInfo(cl_event_info info, size_t size, void* value) const;

    Type type() const;
    virtual void hostWait() = 0;
    virtual void hostSet() = 0;
};

template<EventBase::Type type> class Event;

}
