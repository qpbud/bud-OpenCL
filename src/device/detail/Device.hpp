#pragma once

#include "common/Khronos.hpp"
#include "common/Except.hpp"
namespace qp::cl::detail {

class Device {
    cl_device_type m_type;
public:
    Device(cl_device_type type)
        : m_type(type) {
    }

    virtual ~Device() = default;

    cl_device_type type() const {
        return m_type;
    }

    size_t getInfoSize(cl_device_info info) const {
        switch (info) {
            case CL_DEVICE_TYPE:
                return sizeof(cl_device_type);
            default:
                throw Except(CL_INVALID_VALUE);
        }
    }

    void getInfo(cl_device_info info, size_t size, void* value) const {
        if (size < getInfoSize(info)) {
            throw Except(CL_INVALID_VALUE);
        }

        switch (info) {
            case CL_DEVICE_TYPE:
                *static_cast<cl_device_type*>(value) = m_type;
                break;
            default:
                throw Except(CL_INVALID_VALUE);
        }
    }
};

}
