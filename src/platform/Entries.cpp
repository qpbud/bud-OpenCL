#include "common/Khronos.hpp"
#include "platform/Platform.hpp"

CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformIDs(cl_uint num_entries,
                 cl_platform_id* platforms,
                 cl_uint* num_platforms) CL_API_SUFFIX__VERSION_1_0 {
    if (num_entries == 0 && platforms) {
        return CL_INVALID_VALUE;
    }

    if (!platforms && !num_platforms) {
        return CL_INVALID_VALUE;
    }

    try {
        if (num_platforms) {
            *num_platforms = qp::cl::Platform::count();
        }

        if (num_entries > 0 && platforms) {
            for (cl_uint i = 0; i < num_entries; i++) {
                platforms[i] = &qp::cl::Platform::get(i);
            }
        }
    } catch (const std::exception& e) {
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
