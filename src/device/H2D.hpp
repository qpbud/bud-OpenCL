#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "device/Device.hpp"

namespace qp::cl {

template<typename Detail>
class H1D1 {
protected:
    Device& m_device;
    std::unique_ptr<Detail> m_detail;
public:
    template<typename ... Args>
    H1D1(Device& device, Args&&... args)
        : m_device(device)
        , m_detail() {
        m_detail = detail::Device::Creator<Detail>()(m_device, std::forward<Args>(args)...);
    }

    operator Detail&() { return *m_detail; }
};

template<typename Detail>
class H1DN {
    std::vector<Device*> m_devices;
    std::vector<std::unique_ptr<Detail>> m_details;
public:
    template<typename ... Args>
    void append(Device& device, Args&&... args) {
        m_devices.push_back(&device);
        m_details.push_back(detail::Device::Creator<Detail>()(device, std::forward<Args>(args)...));
    }

    cl_uint getDeviceCount() const {
        return static_cast<cl_uint>(m_devices.size());
    }

    Device& getDevice(cl_uint index) {
        return *m_devices[index];
    }

    Detail& operator[](Device& device) {
        return *m_details[std::distance(m_devices.begin(), std::find(m_devices.begin(), m_devices.end(), &device))];
    }
};

}
