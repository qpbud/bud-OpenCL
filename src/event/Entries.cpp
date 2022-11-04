#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "context/Context.hpp"
#include "event/EventHost.hpp"
#include "event/EventDevice.hpp"

CL_API_ENTRY cl_event CL_API_CALL
clCreateUserEvent(cl_context context,
                  cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!context || !context->isValid()) {
            throw qp::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& contextInternal = static_cast<qp::cl::Context&>(*context);
        cl_event event = &contextInternal.create<qp::cl::Event<qp::cl::EventBase::Type::host>>();

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return event;
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
}

CL_API_ENTRY cl_int CL_API_CALL
clSetUserEventStatus(cl_event event,
                     cl_int execution_status) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!event || !event->isValid()) {
            throw qp::cl::Except(CL_INVALID_EVENT);
        }

        auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event);
        if (eventBaseInternal.type() != qp::cl::EventBase::Type::host) {
            throw qp::cl::Except(CL_INVALID_EVENT);
        }

        if (execution_status != CL_COMPLETE && !(execution_status < 0)) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        auto& eventHostInternal = static_cast<qp::cl::Event<qp::cl::EventBase::Type::host>&>(eventBaseInternal);
        eventHostInternal.setUserStatus(execution_status);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clWaitForEvents(cl_uint num_events,
                const cl_event* event_list) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (num_events == 0 || !event_list) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        qp::cl::EventBase* toCompareEvent = nullptr;
        for (cl_uint i = 0; i < num_events; i++) {
            if (!event_list[i] || !event_list[i]->isValid()) {
                throw qp::cl::Except(CL_INVALID_EVENT);
            }
            auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event_list[i]);
            if (!toCompareEvent) {
                toCompareEvent = &eventBaseInternal;
            } else {
                if (!toCompareEvent->canInterOp(eventBaseInternal)) {
                    throw qp::cl::Except(CL_INVALID_CONTEXT);
                }
            }
        }

        for (cl_uint i = 0; i < num_events; i++) {
            auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event_list[i]);
            eventBaseInternal.hostWait();
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
clGetEventInfo(cl_event event,
               cl_event_info param_name,
               size_t param_value_size,
               void* param_value,
               size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!event || !event->isValid()) {
            throw qp::cl::Except(CL_INVALID_EVENT);
        }

        auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event);

        if (param_value) {
            eventBaseInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = eventBaseInternal.getInfoSize(param_name);
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
clSetEventCallback(cl_event event,
                   cl_int command_exec_callback_type,
                   void (CL_CALLBACK* pfn_notify)(cl_event event,
                                                  cl_int event_command_status,
                                                  void* user_data),
                   void* user_data) CL_API_SUFFIX__VERSION_1_1 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!event || !event->isValid()) {
            throw qp::cl::Except(CL_INVALID_EVENT);
        }

        auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event);
        eventBaseInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!event || !event->isValid()) {
            throw qp::cl::Except(CL_INVALID_EVENT);
        }

        auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event);
        eventBaseInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetEventProfilingInfo(cl_event event,
                        cl_profiling_info param_name,
                        size_t param_value_size,
                        void* param_value,
                        size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}
