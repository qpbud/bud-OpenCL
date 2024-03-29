#include <vector>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "common/Struct.hpp"
#include "queue/QueueBase.hpp"
#include "queue/QueueHost.hpp"
#include "memory/MemoryBase.hpp"
#include "memory/MemoryBuffer.hpp"
#include "event/EventBase.hpp"
#include "event/EventHost.hpp"
#include "event/EventDevice.hpp"

static bool isOverlap(const bud::cl::CopyRegion& copyRegion) {
    size_t rowPitch = copyRegion.src.rowPitch;
    size_t slicePitch = copyRegion.src.slicePitch;
    size_t sliceSize = (copyRegion.region[1] - 1) * rowPitch + copyRegion.region[0];
    size_t blockSize = (copyRegion.region[2] - 1) * slicePitch + sliceSize;
    size_t srcStart = copyRegion.src.origin[2] * slicePitch
                    + copyRegion.src.origin[1] * rowPitch
                    + copyRegion.src.origin[0];
    size_t srcEnd = srcStart + blockSize;
    size_t dstStart = copyRegion.dst.origin[2] * slicePitch
                    + copyRegion.dst.origin[1] * rowPitch
                    + copyRegion.dst.origin[0];
    size_t dstEnd = dstStart + blockSize;

    if (dstEnd <= srcStart || srcEnd <= dstStart) {
        return false;
    }

    size_t srcDx = copyRegion.src.origin[0] % rowPitch;
    size_t dstDx = copyRegion.dst.origin[0] % rowPitch;
    if ((dstDx >= srcDx + copyRegion.region[0] && dstDx + copyRegion.region[0] <= srcDx + rowPitch) ||
        (srcDx >= dstDx + copyRegion.region[0] && srcDx + copyRegion.region[0] <= dstDx + rowPitch)) {
        return false;
    }

    size_t srcDy = (copyRegion.src.origin[1] * rowPitch + copyRegion.src.origin[0]) % slicePitch;
    size_t dstDy = (copyRegion.dst.origin[1] * rowPitch + copyRegion.dst.origin[0]) % slicePitch;
    if ((dstDy >= srcDy + sliceSize && dstDy + sliceSize <= srcDy + slicePitch) ||
        (srcDy >= dstDy + sliceSize && srcDy + sliceSize <= dstDy + slicePitch)) {
        return false;
    }

    return true;
}

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
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if (offset + size > memoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_READ_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, offset, size, ptr);

        if (blocking_read) {
            eventHostInternal.hostWait();
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
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if (offset + size > memoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_WRITE_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, offset, size, ptr);

        if (blocking_write) {
            eventHostInternal.hostWait();
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
clEnqueueReadBufferRect(cl_command_queue command_queue,
                        cl_mem buffer,
                        cl_bool blocking_read,
                        const size_t* buffer_origin,
                        const size_t* host_origin,
                        const size_t* region,
                        size_t buffer_row_pitch,
                        size_t buffer_slice_pitch,
                        size_t host_row_pitch,
                        size_t host_slice_pitch,
                        void* ptr,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        if (!buffer_origin || !host_origin || !region) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (region[0] == 0 || region[1] == 0 || region[2] == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (buffer_row_pitch != 0 && buffer_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t bufferRowPitch = buffer_row_pitch == 0 ? region[0] : buffer_row_pitch;
        if (buffer_slice_pitch != 0 && (buffer_slice_pitch < region[1] * bufferRowPitch || buffer_slice_pitch % bufferRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t bufferSlicePitch = buffer_slice_pitch == 0 ? region[1] * bufferRowPitch : buffer_slice_pitch;
        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if ((buffer_origin[2] + region[2] - 1) * bufferSlicePitch +
            (buffer_origin[1] + region[1] - 1) * bufferRowPitch +
            buffer_origin[0] + region[0] > memoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (host_row_pitch != 0 && host_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t hostRowPitch = host_row_pitch == 0 ? region[0] : host_row_pitch;
        if (host_slice_pitch != 0 && (host_slice_pitch < region[1] * hostRowPitch || host_slice_pitch % hostRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t hostSlicePitch = host_slice_pitch == 0 ? region[1] * hostRowPitch : host_slice_pitch;

        if (!ptr) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        bud::cl::CopyRegion copyRegion{
            {{buffer_origin[0], buffer_origin[1], buffer_origin[2]}, bufferRowPitch, bufferSlicePitch},
            {{host_origin[0], host_origin[1], host_origin[2]}, hostRowPitch, hostSlicePitch},
            {region[0], region[1], region[2]}};
        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_READ_BUFFER_RECT>(
            std::move(waitList), toSetEvent, memoryBufferInternal, ptr, copyRegion);

        if (blocking_read) {
            eventHostInternal.hostWait();
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
clEnqueueWriteBufferRect(cl_command_queue command_queue,
                         cl_mem buffer,
                         cl_bool blocking_write,
                         const size_t* buffer_origin,
                         const size_t* host_origin,
                         const size_t* region,
                         size_t buffer_row_pitch,
                         size_t buffer_slice_pitch,
                         size_t host_row_pitch,
                         size_t host_slice_pitch,
                         const void* ptr,
                         cl_uint num_events_in_wait_list,
                         const cl_event* event_wait_list,
                         cl_event* event) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        if (!buffer_origin || !host_origin || !region) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (region[0] == 0 || region[1] == 0 || region[2] == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (buffer_row_pitch != 0 && buffer_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t bufferRowPitch = buffer_row_pitch == 0 ? region[0] : buffer_row_pitch;
        if (buffer_slice_pitch != 0 && (buffer_slice_pitch < region[1] * bufferRowPitch || buffer_slice_pitch % bufferRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t bufferSlicePitch = buffer_slice_pitch == 0 ? region[1] * bufferRowPitch : buffer_slice_pitch;
        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if ((buffer_origin[2] + region[2] - 1) * bufferSlicePitch +
            (buffer_origin[1] + region[1] - 1) * bufferRowPitch +
            buffer_origin[0] + region[0] > memoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (host_row_pitch != 0 && host_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t hostRowPitch = host_row_pitch == 0 ? region[0] : host_row_pitch;
        if (host_slice_pitch != 0 && (host_slice_pitch < region[1] * hostRowPitch || host_slice_pitch % hostRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t hostSlicePitch = host_slice_pitch == 0 ? region[1] * hostRowPitch : host_slice_pitch;

        if (!ptr) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        bud::cl::CopyRegion copyRegion{
            {{host_origin[0], host_origin[1], host_origin[2]}, hostRowPitch, hostSlicePitch},
            {{buffer_origin[0], buffer_origin[1], buffer_origin[2]}, bufferRowPitch, bufferSlicePitch},
            {region[0], region[1], region[2]}};
        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        queueHostInternal.enqueueCommand<CL_COMMAND_WRITE_BUFFER_RECT>(
            std::move(waitList), toSetEvent, memoryBufferInternal, ptr, copyRegion);

        if (blocking_write) {
            eventHostInternal.hostWait();
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
clEnqueueCopyBuffer(cl_command_queue command_queue,
                    cl_mem src_buffer,
                    cl_mem dst_buffer,
                    size_t src_offset,
                    size_t dst_offset,
                    size_t size,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!src_buffer || !src_buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& srcMemoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*src_buffer);
        if (srcMemoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!dst_buffer || !dst_buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& dstMemoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*dst_buffer);
        if (dstMemoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(srcMemoryBaseInternal) ||
            !queueBaseInternal.canInterOp(dstMemoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& srcMemoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(srcMemoryBaseInternal);
        if (src_offset + size > srcMemoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        auto& dstMemoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(dstMemoryBaseInternal);
        if (dst_offset + size > dstMemoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (&srcMemoryBufferInternal == &dstMemoryBufferInternal &&
            ((src_offset <= dst_offset && dst_offset <= src_offset + size - 1) ||
             (dst_offset <= src_offset && src_offset <= dst_offset + size - 1))) {
            throw bud::cl::Except(CL_MEM_COPY_OVERLAP);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        auto& eventDeviceInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::device>>(queueHostInternal.getDevice());
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventDeviceInternal};
        if (event) {
            *event = &eventDeviceInternal;
        } else {
            toSetEvent->release();
        }

        bud::cl::CopyRegion copyRegion{
            {{src_offset, 0, 0}, size, size},
            {{dst_offset, 0, 0}, size, size},
            {size, 1, 1}};
        queueHostInternal.enqueueCommand<CL_COMMAND_COPY_BUFFER>(
            std::move(waitList), toSetEvent, srcMemoryBufferInternal, dstMemoryBufferInternal, copyRegion);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBufferRect(cl_command_queue command_queue,
                        cl_mem src_buffer,
                        cl_mem dst_buffer,
                        const size_t* src_origin,
                        const size_t* dst_origin,
                        const size_t* region,
                        size_t src_row_pitch,
                        size_t src_slice_pitch,
                        size_t dst_row_pitch,
                        size_t dst_slice_pitch,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!src_buffer || !src_buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& srcMemoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*src_buffer);
        if (srcMemoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!dst_buffer || !dst_buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& dstMemoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*dst_buffer);
        if (dstMemoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(srcMemoryBaseInternal) ||
            !queueBaseInternal.canInterOp(dstMemoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        if (!src_origin || !dst_origin || !region) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (region[0] == 0 || region[1] == 0 || region[2] == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (src_row_pitch != 0 && src_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t srcRowPitch = src_row_pitch == 0 ? region[0] : src_row_pitch;
        if (src_slice_pitch != 0 && (src_slice_pitch < region[1] * srcRowPitch || src_slice_pitch % srcRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t srcSlicePitch = src_slice_pitch == 0 ? region[1] * srcRowPitch : src_slice_pitch;
        auto& srcMemoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(srcMemoryBaseInternal);
        if ((src_origin[2] + region[2] - 1) * srcSlicePitch +
            (src_origin[1] + region[1] - 1) * srcRowPitch +
            src_origin[0] + region[0] > srcMemoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (dst_row_pitch != 0 && dst_row_pitch < region[0]) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t dstRowPitch = dst_row_pitch == 0 ? region[0] : dst_row_pitch;
        if (dst_slice_pitch != 0 && (dst_slice_pitch < region[1] * dstRowPitch || dst_slice_pitch % dstRowPitch != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        size_t dstSlicePitch = dst_slice_pitch == 0 ? region[1] * dstRowPitch : dst_slice_pitch;
        auto& dstMemoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(dstMemoryBaseInternal);
        if ((dst_origin[2] + region[2] - 1) * dstSlicePitch +
            (dst_origin[1] + region[1] - 1) * dstRowPitch +
            dst_origin[0] + region[0] > dstMemoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        bud::cl::CopyRegion copyRegion{
            {{src_origin[0], src_origin[1], src_origin[2]}, srcRowPitch, srcSlicePitch},
            {{dst_origin[0], dst_origin[1], dst_origin[2]}, dstRowPitch, dstSlicePitch},
            {region[0], region[1], region[2]}};
        if (&srcMemoryBufferInternal == &dstMemoryBufferInternal) {
            if (srcRowPitch != dstRowPitch || srcSlicePitch != dstSlicePitch) {
                throw bud::cl::Except(CL_INVALID_VALUE);
            }
            if (isOverlap(copyRegion)) {
                throw bud::cl::Except(CL_MEM_COPY_OVERLAP);
            }
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        auto& eventDeviceInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::device>>(queueHostInternal.getDevice());
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventDeviceInternal};
        if (event) {
            *event = &eventDeviceInternal;
        } else {
            toSetEvent->release();
        }

        queueHostInternal.enqueueCommand<CL_COMMAND_COPY_BUFFER_RECT>(
            std::move(waitList), toSetEvent, srcMemoryBufferInternal, dstMemoryBufferInternal, copyRegion);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueFillBuffer(cl_command_queue command_queue,
                    cl_mem buffer,
                    const void* pattern,
                    size_t pattern_size,
                    size_t offset,
                    size_t size,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) CL_API_SUFFIX__VERSION_1_2 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if (offset + size > memoryBufferInternal.size()) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (!pattern || (pattern_size != 1 &&
                         pattern_size != 2 &&
                         pattern_size != 4 &&
                         pattern_size != 8 &&
                         pattern_size != 16 &&
                         pattern_size != 32 &&
                         pattern_size != 64 &&
                         pattern_size != 128)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (offset % pattern_size != 0 || size % pattern_size != 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        auto& eventDeviceInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::device>>(queueHostInternal.getDevice());
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventDeviceInternal};
        if (event) {
            *event = &eventDeviceInternal;
        } else {
            toSetEvent->release();
        }

        queueHostInternal.enqueueCommand<CL_COMMAND_FILL_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, pattern, pattern_size, offset, size);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY void* CL_API_CALL
clEnqueueMapBuffer(cl_command_queue command_queue,
                   cl_mem buffer,
                   cl_bool blocking_map,
                   cl_map_flags map_flags,
                   size_t offset,
                   size_t size,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event,
                   cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!command_queue || !command_queue->isValid()) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }
        auto& queueBaseInternal = static_cast<bud::cl::QueueBase&>(*command_queue);
        if (queueBaseInternal.type() != bud::cl::QueueBase::Type::host) {
            throw bud::cl::Except(CL_INVALID_COMMAND_QUEUE);
        }

        if (!buffer || !buffer->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*buffer);
        if (memoryBaseInternal.type() != bud::cl::MemoryBase::Type::buffer) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        if (!queueBaseInternal.canInterOp(memoryBaseInternal)) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& memoryBufferInternal = static_cast<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>&>(memoryBaseInternal);
        if (offset + size > memoryBufferInternal.size() || size == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (map_flags != CL_MAP_READ &&
            map_flags != CL_MAP_WRITE &&
            map_flags != CL_MAP_WRITE_INVALIDATE_REGION) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((!event_wait_list && num_events_in_wait_list > 0) ||
            (event_wait_list && num_events_in_wait_list == 0)) {
            throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
        }
        std::vector<boost::intrusive_ptr<bud::cl::EventBase>> waitList;
        for (cl_uint i = 0; i < num_events_in_wait_list; i++) {
            if (!event_wait_list[i] || !event_wait_list[i]->isValid()) {
                throw bud::cl::Except(CL_INVALID_EVENT_WAIT_LIST);
            }
            auto& eventBaseInternal = static_cast<bud::cl::EventBase&>(*event_wait_list[i]);
            if (!queueBaseInternal.canInterOp(eventBaseInternal)) {
                throw bud::cl::Except(CL_INVALID_CONTEXT);
            }
            waitList.emplace_back(&eventBaseInternal);
        }

        auto& eventHostInternal = queueBaseInternal.getContext().create<bud::cl::Event<bud::cl::EventBase::Type::host>>();
        boost::intrusive_ptr<bud::cl::EventBase> toSetEvent{&eventHostInternal};
        if (event) {
            *event = &eventHostInternal;
        } else {
            toSetEvent->release();
        }

        auto& queueHostInternal = static_cast<bud::cl::Queue<bud::cl::QueueBase::Type::host>&>(queueBaseInternal);
        void* mappedBuffer;
        queueHostInternal.enqueueCommand<CL_COMMAND_MAP_BUFFER>(
            std::move(waitList), toSetEvent, memoryBufferInternal, &mappedBuffer, map_flags, offset, size);

        if (blocking_map) {
            eventHostInternal.hostWait();
        }

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return mappedBuffer;
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
clEnqueueReadImage(cl_command_queue command_queue,
                   cl_mem image,
                   cl_bool blocking_read,
                   const size_t * origin,
                   const size_t * region,
                   size_t row_pitch,
                   size_t slice_pitch,
                   void* ptr,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueWriteImage(cl_command_queue command_queue,
                    cl_mem image,
                    cl_bool blocking_write,
                    const size_t* origin,
                    const size_t* region,
                    size_t input_row_pitch,
                    size_t input_slice_pitch,
                    const void* ptr,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImage(cl_command_queue command_queue,
                   cl_mem src_image,
                   cl_mem dst_image,
                   const size_t* src_origin,
                   const size_t* dst_origin,
                   const size_t* region,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueFillImage(cl_command_queue command_queue,
                   cl_mem image,
                   const void* fill_color,
                   const size_t* origin,
                   const size_t* region,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyImageToBuffer(cl_command_queue command_queue,
                           cl_mem src_image,
                           cl_mem dst_buffer,
                           const size_t* src_origin,
                           const size_t* region,
                           size_t dst_offset,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueCopyBufferToImage(cl_command_queue command_queue,
                           cl_mem src_buffer,
                           cl_mem dst_image,
                           size_t src_offset,
                           const size_t* dst_origin,
                           const size_t* region,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY void* CL_API_CALL
clEnqueueMapImage(cl_command_queue command_queue,
                  cl_mem image,
                  cl_bool blocking_map,
                  cl_map_flags map_flags,
                  const size_t* origin,
                  const size_t* region,
                  size_t* image_row_pitch,
                  size_t* image_slice_pitch,
                  cl_uint num_events_in_wait_list,
                  const cl_event* event_wait_list,
                  cl_event* event,
                  cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueUnmapMemObject(cl_command_queue command_queue,
                        cl_mem memobj,
                        void* mapped_ptr,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list,
                        cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMigrateMemObjects(cl_command_queue command_queue,
                           cl_uint num_mem_objects,
                           const cl_mem* mem_objects,
                           cl_mem_migration_flags flags,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list,
                           cl_event* event) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMFree(cl_command_queue command_queue,
                 cl_uint num_svm_pointers,
                 void* svm_pointers[],
                 void (CL_CALLBACK* pfn_free_func)(cl_command_queue queue,
                                                   cl_uint num_svm_pointers,
                                                   void* svm_pointers[],
                                                   void* user_data),
                 void* user_data,
                 cl_uint num_events_in_wait_list,
                 const cl_event* event_wait_list,
                 cl_event* event) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMMemcpy(cl_command_queue command_queue,
                   cl_bool blocking_copy,
                   void* dst_ptr,
                   const void* src_ptr,
                   size_t size,
                   cl_uint num_events_in_wait_list,
                   const cl_event* event_wait_list,
                   cl_event* event) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMMemFill(cl_command_queue command_queue,
                    void* svm_ptr,
                    const void* pattern,
                    size_t pattern_size,
                    size_t size,
                    cl_uint num_events_in_wait_list,
                    const cl_event* event_wait_list,
                    cl_event* event) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMMap(cl_command_queue command_queue,
                cl_bool blocking_map,
                cl_map_flags flags,
                void* svm_ptr,
                size_t size,
                cl_uint num_events_in_wait_list,
                const cl_event* event_wait_list,
                cl_event* event) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMUnmap(cl_command_queue command_queue,
                  void* svm_ptr,
                  cl_uint num_events_in_wait_list,
                  const cl_event* event_wait_list,
                  cl_event* event) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueSVMMigrateMem(cl_command_queue command_queue,
                       cl_uint num_svm_pointers,
                       const void** svm_pointers,
                       const size_t* sizes,
                       cl_mem_migration_flags flags,
                       cl_uint num_events_in_wait_list,
                       const cl_event* event_wait_list,
                       cl_event* event) CL_API_SUFFIX__VERSION_2_1 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNDRangeKernel(cl_command_queue command_queue,
                       cl_kernel kernel,
                       cl_uint work_dim,
                       const size_t* global_work_offset,
                       const size_t* global_work_size,
                       const size_t* local_work_size,
                       cl_uint num_events_in_wait_list,
                       const cl_event* event_wait_list,
                       cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_2_DEPRECATED cl_int CL_API_CALL
clEnqueueTask(cl_command_queue command_queue,
              cl_kernel kernel,
              cl_uint num_events_in_wait_list,
              const cl_event* event_wait_list,
              cl_event* event) CL_API_SUFFIX__VERSION_1_2_DEPRECATED {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNativeKernel(cl_command_queue command_queue,
                      void (CL_CALLBACK* user_func)(void*),
                      void* args,
                      size_t cb_args,
                      cl_uint num_mem_objects,
                      const cl_mem* mem_list,
                      const void** args_mem_loc,
                      cl_uint num_events_in_wait_list,
                      const cl_event* event_wait_list,
                      cl_event* event) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueMarkerWithWaitList(cl_command_queue command_queue,
                            cl_uint num_events_in_wait_list,
                            const cl_event* event_wait_list,
                            cl_event* event) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueMarker(cl_command_queue command_queue,
                cl_event* event) CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueWaitForEvents(cl_command_queue command_queue,
                       cl_uint num_events,
                       const cl_event* event_list) CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueBarrierWithWaitList(cl_command_queue command_queue,
                             cl_uint num_events_in_wait_list,
                             const cl_event* event_wait_list,
                             cl_event* event) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clEnqueueBarrier(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    return CL_INVALID_OPERATION;
}
