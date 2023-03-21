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
    try {
        std::vector<cl_context_properties> propertiesVec;
        if (properties) {
            while (properties && *properties != 0) {
                switch (*properties) {
                    case CL_CONTEXT_PLATFORM: {
                        auto platform = reinterpret_cast<cl_platform_id>(*(properties + 1));
                        if (!platform || !platform->isValid()) {
                            throw bud::cl::Except(CL_INVALID_PLATFORM);
                        }
                        break;
                    }
                    case CL_CONTEXT_INTEROP_USER_SYNC:
                        [[fallthrough]];
                    default:
                        throw bud::cl::Except(CL_INVALID_PROPERTY);
                }
                propertiesVec.push_back(*properties);
                propertiesVec.push_back(*(properties + 1));
                properties += 2;
            }
            propertiesVec.push_back(0);
        }

        if (num_devices == 0 || !devices) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (!pfn_notify && user_data) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        std::vector<cl_device_id> devicesVec(num_devices);
        for (cl_uint i = 0; i < num_devices; i++) {
            if (!devices[i] || !devices[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_DEVICE);
            }
            devicesVec[i] = devices[i];
        }

        cl_context context = &bud::cl::Context::construct(std::move(propertiesVec), std::move(devicesVec),
            [pfn_notify, user_data](const char* errInfo, const void* privateInfo, size_t privateInfoSize) {
                pfn_notify(errInfo, privateInfo, privateInfoSize, user_data);
            });

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return context;
    } catch (const std::exception& e) {
        if (errcode_ret) {
            if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
                *errcode_ret = except->err();
            } else {
                *errcode_ret = CL_OUT_OF_HOST_MEMORY;
            }
        }
        return nullptr;
    }
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
    try {
        bud::cl::Platform* platformInternal = &bud::cl::Platform::get(0);
        std::vector<cl_context_properties> propertiesVec;
        if (properties) {
            while (properties && *properties != 0) {
                switch (*properties) {
                    case CL_CONTEXT_PLATFORM: {
                        auto platform = reinterpret_cast<cl_platform_id>(*(properties + 1));
                        if (!platform || !platform->isValid()) {
                            throw bud::cl::Except(CL_INVALID_PLATFORM);
                        }
                        platformInternal = static_cast<bud::cl::Platform*>(platform);
                        break;
                    }
                    case CL_CONTEXT_INTEROP_USER_SYNC:
                        [[fallthrough]];
                    default:
                        throw bud::cl::Except(CL_INVALID_PROPERTY);
                }
                propertiesVec.push_back(*properties);
                propertiesVec.push_back(*(properties + 1));
                properties += 2;
            }
            propertiesVec.push_back(0);
        }

        if (!pfn_notify && user_data) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (device_type != CL_DEVICE_TYPE_CPU &&
            device_type != CL_DEVICE_TYPE_GPU &&
            device_type != CL_DEVICE_TYPE_ACCELERATOR &&
            device_type != CL_DEVICE_TYPE_CUSTOM &&
            device_type != CL_DEVICE_TYPE_DEFAULT &&
            device_type != CL_DEVICE_TYPE_ALL) {
            throw bud::cl::Except(CL_INVALID_DEVICE_TYPE);
        }

        std::vector<cl_device_id> devicesVec;
        for (cl_uint i = 0; i < platformInternal->getDeviceCount(); i++) {
            if (bud::cl::Device& device = platformInternal->getDevice(i);
                device_type == CL_DEVICE_TYPE_DEFAULT ||
                device_type == CL_DEVICE_TYPE_ALL ||
                device_type == device.type()) {
                devicesVec.push_back(&device); // only create from one device
                break;
            }
        }

        if (devicesVec.empty()) {
            throw bud::cl::Except(CL_DEVICE_NOT_FOUND);
        }

        cl_context context = &bud::cl::Context::construct(std::move(propertiesVec), std::move(devicesVec),
            [pfn_notify, user_data](const char* errInfo, const void* privateInfo, size_t privateInfoSize) {
                pfn_notify(errInfo, privateInfo, privateInfoSize, user_data);
            });

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return context;
    } catch (const std::exception& e) {
        if (errcode_ret) {
            if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
                *errcode_ret = except->err();
            } else {
                *errcode_ret = CL_OUT_OF_HOST_MEMORY;
            }
        }
        return nullptr;
    }
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainContext(cl_context context) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        contextInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseContext(cl_context context) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        contextInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetContextInfo(cl_context context,
                 cl_context_info param_name,
                 size_t param_value_size,
                 void* param_value,
                 size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);

        if (param_value) {
            contextInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = contextInternal.getInfoSize(param_name);
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
clSetContextDestructorCallback(cl_context context,
                               void (CL_CALLBACK* pfn_notify)(cl_context context,
                                                              void* user_data),
                               void* user_data) CL_API_SUFFIX__VERSION_3_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!pfn_notify) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        contextInternal.setDestructorCallback([pfn_notify, context, user_data] { pfn_notify(context, user_data); });
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
