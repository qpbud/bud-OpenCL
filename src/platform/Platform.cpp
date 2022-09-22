#include <cstring>
#include "common/Except.hpp"
#include "platform/Platform.hpp"

namespace qp::cl {

std::vector<Platform> Platform::s_platforms{};
std::once_flag Platform::s_initFlag{};

template<>
Platform::Platform(QP)
    : Object<_cl_platform_id>()
    , m_profile("EMBEDDED_PROFILE")
    , m_version("OpenCL 3.0")
    , m_numericVersion(CL_MAKE_VERSION_KHR(3, 0, 0))
    , m_name("QP")
    , m_vendor("BUD")
    , m_extensions("")
    , m_extensionsWithVersion()
    , m_hostTimerResolution(0)
    , m_devices() {
}

template<> 
void Platform::createDevices<Platform::QP>() {
    m_devices.emplace_back(Device::Vulkan());
}

void Platform::init() {
    s_platforms.emplace_back(QP());
    s_platforms.back().createDevices<QP>();
}

cl_uint Platform::count() {
    std::call_once(s_initFlag, &Platform::init);
    return static_cast<cl_uint>(s_platforms.size());
}

Platform& Platform::get(cl_uint index) {
    std::call_once(s_initFlag, &Platform::init);
    return s_platforms[index];
}

size_t Platform::getInfoSize(cl_platform_info info) const {
    switch (info) {
        case CL_PLATFORM_PROFILE:
            return m_profile.size() + 1;
        case CL_PLATFORM_VERSION:
            return m_version.size() + 1;
        case CL_PLATFORM_NUMERIC_VERSION:
            return sizeof(cl_version);
        case CL_PLATFORM_NAME:
            return m_name.size() + 1;
        case CL_PLATFORM_VENDOR:
            return m_vendor.size() + 1;
        case CL_PLATFORM_EXTENSIONS:
            return m_extensions.size() + 1;
        case CL_PLATFORM_EXTENSIONS_WITH_VERSION:
            return m_extensionsWithVersion.size() * sizeof(cl_name_version);
        case CL_PLATFORM_HOST_TIMER_RESOLUTION:
            return sizeof(cl_ulong);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Platform::getInfo(cl_platform_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_PLATFORM_PROFILE:
            m_profile.copy(static_cast<char*>(value), m_profile.size());
            static_cast<char*>(value)[m_profile.size()] = '\0';
            break;
        case CL_PLATFORM_VERSION:
            m_version.copy(static_cast<char*>(value), m_version.size());
            static_cast<char*>(value)[m_version.size()] = '\0';
            break;
        case CL_PLATFORM_NUMERIC_VERSION:
            *static_cast<cl_version*>(value) = m_numericVersion;
            break;
        case CL_PLATFORM_NAME:
            m_name.copy(static_cast<char*>(value), m_name.size());
            static_cast<char*>(value)[m_name.size()] = '\0';
            break;
        case CL_PLATFORM_VENDOR:
            m_vendor.copy(static_cast<char*>(value), m_vendor.size());
            static_cast<char*>(value)[m_vendor.size()] = '\0';
            break;
        case CL_PLATFORM_EXTENSIONS:
            m_extensions.copy(static_cast<char*>(value), m_extensions.size());
            static_cast<char*>(value)[m_extensions.size()] = '\0';
            break;
        case CL_PLATFORM_EXTENSIONS_WITH_VERSION:
            std::memcpy(value, m_extensionsWithVersion.data(), m_extensionsWithVersion.size() * sizeof(cl_name_version));
        case CL_PLATFORM_HOST_TIMER_RESOLUTION:
            *static_cast<cl_ulong*>(value) = m_hostTimerResolution;
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

cl_uint Platform::getDeviceCount() const {
    return static_cast<cl_uint>(m_devices.size());
}

Device& Platform::getDevice(cl_uint index) {
    return m_devices[index];
}

}
