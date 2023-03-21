#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "queue/QueueBase.hpp"
#include "queue/QueueHost.hpp"

CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueueWithProperties(cl_context context,
                                   cl_device_id device,
                                   const cl_queue_properties* properties,
                                   cl_int* errcode_ret) CL_API_SUFFIX__VERSION_2_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!device || !device->isValid()) {
            throw bud::cl::Except(CL_INVALID_DEVICE);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        auto& deviceInternal = static_cast<bud::cl::Device&>(*device);
        bool containsDevice = false;
        for (cl_uint i = 0; i < contextInternal.getDeviceCount(); i++) {
            if (&contextInternal.getDevice(i) == &deviceInternal) {
                containsDevice = true;
                break;
            }
        }
        if (!containsDevice) {
            throw bud::cl::Except(CL_INVALID_DEVICE);
        }

        std::vector<cl_queue_properties> propertiesVec;
        cl_command_queue_properties propertyBits;
        bool enableProfiling = false;
        if (properties) {
            while (properties && *properties != 0) {
                switch (*properties) {
                    case CL_QUEUE_PROPERTIES: {
                        if (*(properties + 1) &
                            (~(CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE
                             | CL_QUEUE_PROFILING_ENABLE
                             | CL_QUEUE_ON_DEVICE
                             | CL_QUEUE_ON_DEVICE_DEFAULT)) != 0) {
                            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE != 0) {
                            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_PROFILING_ENABLE != 0) {
                            enableProfiling = true;
                        }
                        if (*(properties + 1) & CL_QUEUE_ON_DEVICE != 0) {
                            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_ON_DEVICE_DEFAULT != 0) {
                            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        propertyBits = *(properties + 1);
                        break;
                    }
                    case CL_QUEUE_SIZE:
                        [[fallthrough]];
                    default:
                        throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                }
                propertiesVec.push_back(*properties);
                propertiesVec.push_back(*(properties + 1));
                properties += 2;
            }
            propertiesVec.push_back(0);
        }

        cl_command_queue queue = &contextInternal.create<bud::cl::Queue<bud::cl::QueueBase::Type::host>>(
            deviceInternal, std::move(propertiesVec), propertyBits, enableProfiling);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return queue;
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

CL_API_ENTRY CL_API_PREFIX__VERSION_1_2_DEPRECATED cl_command_queue CL_API_CALL
clCreateCommandQueue(cl_context context,
                     cl_device_id device,
                     cl_command_queue_properties properties,
                     cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_2_DEPRECATED {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!device || !device->isValid()) {
            throw bud::cl::Except(CL_INVALID_DEVICE);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        auto& deviceInternal = static_cast<bud::cl::Device&>(*device);
        bool containsDevice = false;
        for (cl_uint i = 0; i < contextInternal.getDeviceCount(); i++) {
            if (&contextInternal.getDevice(i) == &deviceInternal) {
                containsDevice = true;
                break;
            }
        }
        if (!containsDevice) {
            throw bud::cl::Except(CL_INVALID_DEVICE);
        }

        bool enableProfiling = false;
        if (properties & (~(CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE)) != 0) {
            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
        }
        if (properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE != 0) {
            throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
        }
        if (properties & CL_QUEUE_PROFILING_ENABLE != 0) {
            enableProfiling = true;
        }

        cl_command_queue queue = &contextInternal.create<bud::cl::Queue<bud::cl::QueueBase::Type::host>>(
            deviceInternal, std::vector<cl_queue_properties>(), properties, enableProfiling);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return queue;
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
clSetDefaultDeviceCommandQueue(cl_context context,
                               cl_device_id device,
                               cl_command_queue command_queue) CL_API_SUFFIX__VERSION_2_1 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        queueBaseInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        queueBaseInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetCommandQueueInfo(cl_command_queue command_queue,
                      cl_command_queue_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);

        if (param_value) {
            queueBaseInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = queueBaseInternal.getInfoSize(param_name);
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
clSetCommandQueueProperty(cl_command_queue command_queue,
                          cl_command_queue_properties properties,
                          cl_bool enable,
                          cl_command_queue_properties* old_properties) CL_API_SUFFIX__VERSION_1_0_DEPRECATED {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clFlush(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.flush();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clFinish(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.finish();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
