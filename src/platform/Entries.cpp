#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "platform/Platform.hpp"

CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformIDs(cl_uint num_entries,
                 cl_platform_id* platforms,
                 cl_uint* num_platforms) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (num_entries == 0 && platforms) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (!platforms && !num_platforms) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (num_platforms) {
            *num_platforms = qp::cl::Platform::count();
        }

        if (platforms) {
            for (cl_uint i = 0; i < num_entries; i++) {
                platforms[i] = &qp::cl::Platform::get(i);
            }
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformInfo(cl_platform_id platform,
                  cl_platform_info param_name,
                  size_t param_value_size,
                  void* param_value,
                  size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!platform || !platform->isValid()) {
            throw qp::cl::Except(CL_INVALID_PLATFORM);
        }

        auto& platformInternal = static_cast<qp::cl::Platform&>(*platform);

        if (param_value) {
            platformInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = platformInternal.getInfoSize(param_name);
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
