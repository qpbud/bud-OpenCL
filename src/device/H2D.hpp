#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "device/Device.hpp"

namespace bud::cl {

template<typename Hal>
class H1D1 {
protected:
    Device& m_device;
    std::unique_ptr<Hal> m_hal;
public:
    template<typename ... Args>
    H1D1(Device& device, Args&&... args)
        : m_device(device)
        , m_hal() {
        m_hal = hal::Device::Creator<Hal>()(m_device, std::forward<Args>(args)...);
    }

    Device& getDevice() {
        return m_device;
    }

    operator Hal&() { return *m_hal; }
};

template<typename Hal>
class H1DN {
    std::vector<Device*> m_devices;
    std::vector<std::unique_ptr<Hal>> m_hals;
public:
    template<typename ... Args>
    void append(Device& device, Args&&... args) {
        m_devices.push_back(&device);
        m_hals.push_back(hal::Device::Creator<Hal>()(device, std::forward<Args>(args)...));
    }

    cl_uint getDeviceCount() const {
        return static_cast<cl_uint>(m_devices.size());
    }

    Device& getDevice(cl_uint index) {
        return *m_devices[index];
    }

    Hal& operator[](Device& device) {
        return *m_hals[std::distance(m_devices.begin(), std::find(m_devices.begin(), m_devices.end(), &device))];
    }
};

}
