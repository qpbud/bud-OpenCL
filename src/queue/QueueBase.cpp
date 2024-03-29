#include <cstring>
#include "queue/QueueBase.hpp"

namespace bud::cl {

QueueBase::QueueBase(Context& context,
                     QueueBase::Type type,
                     cl_device_id deviceId,
                     std::vector<cl_queue_properties>&& properties,
                     cl_command_queue_properties propertyBits)
    : Object<_cl_command_queue>()
    , Scope(context)
    , m_type(type)
    , m_deviceId(deviceId)
    , m_properties(std::move(properties))
    , m_propertyBits(propertyBits) {}

size_t QueueBase::getInfoSize(cl_command_queue_info info) const {
    switch (info) {
        case CL_QUEUE_CONTEXT:
            return sizeof(cl_context);
        case CL_QUEUE_DEVICE:
            return sizeof(cl_device_id);
        case CL_QUEUE_REFERENCE_COUNT:
            return sizeof(cl_uint);
        case CL_QUEUE_PROPERTIES:
            return sizeof(cl_command_queue_properties);
        case CL_QUEUE_PROPERTIES_ARRAY:
            return sizeof(cl_queue_properties) * m_properties.size();
        case CL_QUEUE_SIZE:
            [[fallthrough]];
        case CL_QUEUE_DEVICE_DEFAULT:
            throw Except(CL_INVALID_QUEUE_PROPERTIES);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void QueueBase::getInfo(cl_command_queue_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_QUEUE_CONTEXT:
            *static_cast<cl_context*>(value) = m_context.get();
            break;
        case CL_QUEUE_DEVICE:
            *static_cast<cl_device_id*>(value) = m_deviceId;
            break;
        case CL_QUEUE_REFERENCE_COUNT:
            *static_cast<cl_uint*>(value) = count();
            break;
        case CL_QUEUE_PROPERTIES:
            *static_cast<cl_command_queue_properties*>(value) = m_propertyBits;
            break;
        case CL_QUEUE_PROPERTIES_ARRAY:
            std::memcpy(value, m_properties.data(), sizeof(cl_queue_properties) * m_properties.size());
            break;
        case CL_QUEUE_SIZE:
            [[fallthrough]];
        case CL_QUEUE_DEVICE_DEFAULT:
            throw Except(CL_INVALID_QUEUE_PROPERTIES);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

QueueBase::Type QueueBase::type() const {
    return m_type;
}

}
