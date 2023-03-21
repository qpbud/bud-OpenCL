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
    try {
        if (!platform || !platform->isValid()) {
            throw bud::cl::Except(CL_INVALID_PLATFORM);
        }

        if (device_type != CL_DEVICE_TYPE_CPU &&
            device_type != CL_DEVICE_TYPE_GPU &&
            device_type != CL_DEVICE_TYPE_ACCELERATOR &&
            device_type != CL_DEVICE_TYPE_CUSTOM &&
            device_type != CL_DEVICE_TYPE_DEFAULT &&
            device_type != CL_DEVICE_TYPE_ALL) {
            throw bud::cl::Except(CL_INVALID_DEVICE_TYPE);
        }

        if (num_entries == 0 && devices) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (!devices && !num_devices) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        auto& platformInternal = static_cast<bud::cl::Platform&>(*platform);

        cl_uint count = 0;
        for (cl_uint i = 0; i < platformInternal.getDeviceCount(); i++) {
            bud::cl::Device& device = platformInternal.getDevice(i);
            if (device_type == CL_DEVICE_TYPE_DEFAULT ||
                device_type == CL_DEVICE_TYPE_ALL ||
                device_type == device.type()) {
                if (devices) {
                    devices[count] = &device;
                }
                count++;
                if (count == num_entries) {
                    break;
                }
            }
        }

        if (count == 0) {
            throw bud::cl::Except(CL_DEVICE_NOT_FOUND);
        }

        if (num_devices) {
            *num_devices = count;
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetDeviceInfo(cl_device_id device,
                cl_device_info param_name,
                size_t param_value_size,
                void* param_value,
                size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!device || !device->isValid()) {
            throw bud::cl::Except(CL_INVALID_DEVICE);
        }

        auto& deviceInternal = static_cast<bud::cl::Device&>(*device);

        if (param_value) {
            deviceInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = deviceInternal.getInfoSize(param_name);
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
