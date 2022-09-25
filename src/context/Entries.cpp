#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "platform/Platform.hpp"
#include "context/Context.hpp"

CL_API_ENTRY cl_context CL_API_CALL
clCreateContext(const cl_context_properties* properties,
                cl_uint num_devices,
                const cl_device_id* devices,
                void (CL_CALLBACK* pfn_notify)(const char* errinfo,
                                               const void* private_info,
                                               size_t cb,
                                               void* user_data),
                void* user_data,
                cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    cl_context context;
    try {
        size_t propLen = 0;
        std::vector<cl_context_properties> propertiesVec;
        while (properties[propLen] != 0) {
            switch (properties[propLen]) {
                case CL_CONTEXT_PLATFORM: {
                    auto platform = reinterpret_cast<cl_platform_id>(properties[propLen + 1]);
                    if (!platform || !platform->isValid()) {
                        throw qp::cl::Except(CL_INVALID_PLATFORM);
                    }
                    break;
                }
                case CL_CONTEXT_INTEROP_USER_SYNC:
                    [[fallthrough]];
                default:
                    throw qp::cl::Except(CL_INVALID_PROPERTY);
            }
            propertiesVec.push_back(properties[propLen]);
            propertiesVec.push_back(properties[propLen + 1]);
            propLen += 2;
        }
        propertiesVec.push_back(0);

        if (num_devices == 0 || !devices) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (!pfn_notify && user_data) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        std::vector<cl_device_id> devicesVec(num_devices);
        for (cl_uint i = 0; i < num_devices; i++) {
            if (!devices[i] || !devices[i]->isValid()) {
                throw qp::cl::Except(CL_INVALID_DEVICE);
            }
            devicesVec[i] = devices[i];
        }

        context = &qp::cl::Context::create(std::move(propertiesVec), std::move(devicesVec), pfn_notify, user_data);
    } catch (const std::exception& e) {
        if (errcode_ret) {
            if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
                *errcode_ret = except->err();
            } else {
                *errcode_ret = CL_OUT_OF_HOST_MEMORY;
            }
        }
        return nullptr;
    }

    if (errcode_ret) {
        *errcode_ret = CL_SUCCESS;
    }
    return context;
}

CL_API_ENTRY cl_context CL_API_CALL
clCreateContextFromType(const cl_context_properties* properties,
                        cl_device_type device_type,
                        void (CL_CALLBACK* pfn_notify)(const char* errinfo,
                                                       const void* private_info,
                                                       size_t cb,
                                                       void* user_data),
                        void* user_data,
                        cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    cl_context context;
    try {
        qp::cl::Platform* platformInternal = &qp::cl::Platform::get(0);
        size_t propLen = 0;
        std::vector<cl_context_properties> propertiesVec;
        while (properties[propLen] != 0) {
            switch (properties[propLen]) {
                case CL_CONTEXT_PLATFORM: {
                    auto platform = reinterpret_cast<cl_platform_id>(properties[propLen + 1]);
                    if (!platform || !platform->isValid()) {
                        throw qp::cl::Except(CL_INVALID_PLATFORM);
                    }
                    platformInternal = static_cast<qp::cl::Platform*>(platform);
                    break;
                }
                case CL_CONTEXT_INTEROP_USER_SYNC:
                    [[fallthrough]];
                default:
                    throw qp::cl::Except(CL_INVALID_PROPERTY);
            }
            propertiesVec.push_back(properties[propLen]);
            propertiesVec.push_back(properties[propLen + 1]);
            propLen += 2;
        }
        propertiesVec.push_back(0);

        if (!pfn_notify && user_data) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (device_type != CL_DEVICE_TYPE_CPU &&
            device_type != CL_DEVICE_TYPE_GPU &&
            device_type != CL_DEVICE_TYPE_ACCELERATOR &&
            device_type != CL_DEVICE_TYPE_CUSTOM &&
            device_type != CL_DEVICE_TYPE_DEFAULT &&
            device_type != CL_DEVICE_TYPE_ALL) {
            throw qp::cl::Except(CL_INVALID_DEVICE_TYPE);
        }

        std::vector<cl_device_id> devicesVec;
        for (cl_uint i = 0; i < platformInternal->getDeviceCount(); i++) {
            if (qp::cl::Device& device = platformInternal->getDevice(i);
                device_type == CL_DEVICE_TYPE_DEFAULT ||
                device_type == CL_DEVICE_TYPE_ALL ||
                device_type == device.type()) {
                devicesVec.push_back(&device); // only create from one device
                break;
            }
        }

        if (devicesVec.empty()) {
            throw qp::cl::Except(CL_DEVICE_NOT_FOUND);
        }

        context = &qp::cl::Context::create(std::move(propertiesVec), std::move(devicesVec), pfn_notify, user_data);
    } catch (const std::exception& e) {
        if (errcode_ret) {
            if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
                *errcode_ret = except->err();
            } else {
                *errcode_ret = CL_OUT_OF_HOST_MEMORY;
            }
        }
        return nullptr;
    }

    if (errcode_ret) {
        *errcode_ret = CL_SUCCESS;
    }
    return context;
}
