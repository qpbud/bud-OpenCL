#include <vector>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "queue/QueueBase.hpp"
#include "queue/QueueHost.hpp"
#include "memory/MemoryBase.hpp"
#include "memory/MemoryBuffer.hpp"
#include "event/EventBase.hpp"
#include "event/EventHost.hpp"

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueReadBuffer(cl_command_queue command_queue,
                    cl_mem buffer,
                    cl_bool blocking_read,
                    size_t offset,
                    size_t size,
                    void* ptr,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw qp::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<qp::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != qp::cl::QueueBase::Type::host) {
            throw qp::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw qp::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<qp::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != qp::cl::MemoryBase::Type::buffer) {
            throw qp::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw qp::cl::Except(CL_INVALID_CONTEXT);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw qp::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<qp::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw qp::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw qp::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<qp::cl::Event<qp::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<qp::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        auto& queueHostInternal = static_cast<qp::cl::Queue<qp::cl::QueueBase::Type::host>&>(queueBaseInternal);
        auto& memoryBufferInternal = static_cast<qp::cl::Memory<qp::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_READ_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, offset, size, ptr);

        if (blocking_read) {
            eventHostInternal.hostWait();
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
clEnqueueWriteBuffer(cl_command_queue command_queue,
                     cl_mem buffer,
                     cl_bool blocking_write,
                     size_t offset,
                     size_t size,
                     const void* ptr,
                     cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list,
                     cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw qp::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<qp::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != qp::cl::QueueBase::Type::host) {
            throw qp::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw qp::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<qp::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != qp::cl::MemoryBase::Type::buffer) {
            throw qp::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw qp::cl::Except(CL_INVALID_CONTEXT);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw qp::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<qp::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw qp::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<qp::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw qp::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<qp::cl::Event<qp::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<qp::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        auto& queueHostInternal = static_cast<qp::cl::Queue<qp::cl::QueueBase::Type::host>&>(queueBaseInternal);
        auto& memoryBufferInternal = static_cast<qp::cl::Memory<qp::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_WRITE_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, offset, size, ptr);

        if (blocking_write) {
            eventHostInternal.hostWait();
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
