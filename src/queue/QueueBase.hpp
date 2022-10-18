#pragma once

#include <vector>
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
    cl_device_id m_deviceId;
    std::vector<cl_queue_properties> m_properties;
    cl_command_queue_properties m_propertyBits;
public:
    QueueBase(Context& context,
              Type type,
              cl_device_id deviceId,
              std::vector<cl_queue_properties>&& properties,
              cl_command_queue_properties propertyBits);
    virtual ~QueueBase() = default;

    size_t getInfoSize(cl_command_queue_info info) const;
    void getInfo(cl_command_queue_info info, size_t size, void* value) const;

    Type type() const;
};

template<QueueBase::Type type> class Queue;

}
