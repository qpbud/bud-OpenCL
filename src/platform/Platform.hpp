#pragma once

#include <vector>
#include <mutex>
#include <stdexcept>
#include <string_view>
#include "common/Khronos.hpp"
#include "common/Object.hpp"

namespace qp::cl {

class Platform : public Object<_cl_platform_id> {
    static std::vector<Platform> s_platforms;
    static std::once_flag s_initFlag;

    static void init();

    std::string_view m_profile;
    std::string_view m_version;
    cl_version m_numericVersion;
    std::string_view m_name;
    std::string_view m_vendor;
    std::string_view m_extensions;
    std::vector<cl_name_version> m_extensionsWithVersion;
    cl_ulong m_hostTimerResolution;
public:
    static cl_uint count();
    static Platform& get(cl_uint index);

    Platform();
    size_t getInfoSize(cl_platform_info info) const;
    void getInfo(cl_platform_info info, size_t size, void* value);
};

}
