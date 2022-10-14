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
    cl_command_queue queue;
    try {
        if (!context || !context->isValid()) {
            throw qp::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!device || !device->isValid()) {
            throw qp::cl::Except(CL_INVALID_DEVICE);
        }

        auto& contextInternal = static_cast<qp::cl::Context&>(*context);
        auto& deviceInternal = static_cast<qp::cl::Device&>(*device);
        bool containsDevice = false;
        for (cl_uint i = 0; i < contextInternal.getDeviceCount(); i++) {
            if (&contextInternal.getDevice(i) == &deviceInternal) {
                containsDevice = true;
                break;
            }
        }
        if (!containsDevice) {
            throw qp::cl::Except(CL_INVALID_DEVICE);
        }

        std::vector<cl_queue_properties> propertiesVec;
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
                            throw qp::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE != 0) {
                            throw qp::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_PROFILING_ENABLE != 0) {
                            enableProfiling = true;
                        }
                        if (*(properties + 1) & CL_QUEUE_ON_DEVICE != 0) {
                            throw qp::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        if (*(properties + 1) & CL_QUEUE_ON_DEVICE_DEFAULT != 0) {
                            throw qp::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                        }
                        break;
                    }
                    case CL_QUEUE_SIZE:
                        [[fallthrough]];
                    default:
                        throw qp::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                }
                propertiesVec.push_back(*properties);
                propertiesVec.push_back(*(properties + 1));
                properties += 2;
            }
            propertiesVec.push_back(0);
        }

        queue = &contextInternal.create<qp::cl::Queue<qp::cl::QueueBase::Type::host>>(
            deviceInternal, std::move(propertiesVec), enableProfiling);
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
    return queue;
}
