#pragma once

#include <memory>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/detail/Device.hpp"

namespace qp::cl {

class Device : public Object<_cl_device_id> {
    cl_device_type m_type;
    std::unique_ptr<detail::Device> m_detail;
public:
    struct Vulkan {};

    template<typename T> Device(T);
    cl_device_type type() const;
    size_t getInfoSize(cl_device_info info) const;
    void getInfo(cl_device_info info, size_t size, void* value) const;
    operator detail::Device&();
};

}
