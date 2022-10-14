#pragma once

#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace qp::cl {

class QueueBase : public Object<_cl_command_queue>, public Scope {
public:
    enum class Type { host, device };
private:
    Type m_type;
public:
    QueueBase(Context& context, Type type);
    virtual ~QueueBase() = default;
};

template<QueueBase::Type type> class Queue;

}
