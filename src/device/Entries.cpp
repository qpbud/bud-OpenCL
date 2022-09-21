#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "platform/Platform.hpp"
#include "device/Device.hpp"

CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceIDs(cl_platform_id platform,
               cl_device_type device_type,
               cl_uint num_entries,
               cl_device_id* devices,
               cl_uint* num_devices) CL_API_SUFFIX__VERSION_1_0 {
    if (!platform || !platform->isValid()) {
        return CL_INVALID_PLATFORM;
    }

    if (device_type != CL_DEVICE_TYPE_CPU &&
        device_type != CL_DEVICE_TYPE_GPU &&
        device_type != CL_DEVICE_TYPE_ACCELERATOR &&
        device_type != CL_DEVICE_TYPE_CUSTOM &&
        device_type != CL_DEVICE_TYPE_DEFAULT &&
        device_type != CL_DEVICE_TYPE_ALL) {
        return CL_INVALID_DEVICE_TYPE;
    }

    if (num_entries == 0 && devices) {
        return CL_INVALID_VALUE;
    }

    if (!devices && !num_devices) {
        return CL_INVALID_VALUE;
    }

    try {
        auto& platformInternal = static_cast<qp::cl::Platform&>(*platform);

        cl_uint count = 0;
        for (cl_uint i = 0; i < platformInternal.getDeviceCount(); i++) {
            qp::cl::Device& device = platformInternal.getDevice(i);
            if (device_type == CL_DEVICE_TYPE_DEFAULT ||
                device_type == CL_DEVICE_TYPE_ALL ||
                device_type == device.type()) {
                if (devices) {
                    devices[count] = &device;
                }
                count++;
            }
        }

        if (count == 0) {
            throw qp::cl::Except(CL_DEVICE_NOT_FOUND);
        }

        if (num_devices) {
            *num_devices = count;
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
