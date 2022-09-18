#pragma once

#include <vector>
#include <mutex>
#include "common/Khronos.hpp"
#include "common/Object.hpp"

namespace qp::cl {

class Platform : public _cl_platform_id {
    static std::vector<Platform> s_platforms;
    static std::once_flag s_initFlag;

    static void init();
public:
    static cl_uint count();
    static Platform& get(cl_uint index);

    Platform();
};

}
