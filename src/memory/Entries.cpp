#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "context/Context.hpp"
#include "memory/MemoryBase.hpp"
#include "memory/MemoryBuffer.hpp"

CL_API_ENTRY cl_mem CL_API_CALL
clCreateBuffer(cl_context context,
               cl_mem_flags flags,
               size_t size,
               void* host_ptr,
               cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (flags & (~(CL_MEM_READ_WRITE |
                       CL_MEM_WRITE_ONLY |
                       CL_MEM_READ_ONLY |
                       CL_MEM_USE_HOST_PTR |
                       CL_MEM_ALLOC_HOST_PTR |
                       CL_MEM_COPY_HOST_PTR |
                       CL_MEM_HOST_WRITE_ONLY |
                       CL_MEM_HOST_READ_ONLY |
                       CL_MEM_HOST_NO_ACCESS |
                       CL_MEM_KERNEL_READ_AND_WRITE)) != 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_WRITE_ONLY != 0) ||
            (flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_READ_ONLY != 0) ||
            (flags & CL_MEM_WRITE_ONLY != 0 && flags & CL_MEM_READ_ONLY != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (size == 0) {
            throw bud::cl::Except(CL_INVALID_BUFFER_SIZE);
        }

        if (!host_ptr && (flags & CL_MEM_USE_HOST_PTR != 0 || flags & CL_MEM_COPY_HOST_PTR != 0)) {
            throw bud::cl::Except(CL_INVALID_HOST_PTR);
        }

        if (host_ptr && (flags & CL_MEM_USE_HOST_PTR == 0 && flags & CL_MEM_COPY_HOST_PTR == 0)) {
            throw bud::cl::Except(CL_INVALID_HOST_PTR);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);

        cl_mem buffer = &contextInternal.create<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>>(
            std::vector<cl_mem_properties>(), flags, host_ptr, size);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return buffer;
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

CL_API_ENTRY cl_mem CL_API_CALL
clCreateBufferWithProperties(cl_context context,
                             const cl_mem_properties* properties,
                             cl_mem_flags flags,
                             size_t size,
                             void* host_ptr,
                             cl_int* errcode_ret) CL_API_SUFFIX__VERSION_3_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        std::vector<cl_mem_properties> propertiesVec;
        if (properties) {
            while (properties && *properties != 0) {
                switch (*properties) {
                    default:
                        throw bud::cl::Except(CL_INVALID_PROPERTY);
                }
                properties += 2;
            }
            propertiesVec.push_back(0);
        }

        if (flags & (~(CL_MEM_READ_WRITE |
                       CL_MEM_WRITE_ONLY |
                       CL_MEM_READ_ONLY |
                       CL_MEM_USE_HOST_PTR |
                       CL_MEM_ALLOC_HOST_PTR |
                       CL_MEM_COPY_HOST_PTR |
                       CL_MEM_HOST_WRITE_ONLY |
                       CL_MEM_HOST_READ_ONLY |
                       CL_MEM_HOST_NO_ACCESS |
                       CL_MEM_KERNEL_READ_AND_WRITE)) != 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if ((flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_WRITE_ONLY != 0) ||
            (flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_READ_ONLY != 0) ||
            (flags & CL_MEM_WRITE_ONLY != 0 && flags & CL_MEM_READ_ONLY != 0)) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        if (size == 0) {
            throw bud::cl::Except(CL_INVALID_BUFFER_SIZE);
        }

        if (!host_ptr && (flags & CL_MEM_USE_HOST_PTR != 0 || flags & CL_MEM_COPY_HOST_PTR != 0)) {
            throw bud::cl::Except(CL_INVALID_HOST_PTR);
        }

        if (host_ptr && (flags & CL_MEM_USE_HOST_PTR == 0 && flags & CL_MEM_COPY_HOST_PTR == 0)) {
            throw bud::cl::Except(CL_INVALID_HOST_PTR);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);

        cl_mem buffer = &contextInternal.create<bud::cl::Memory<bud::cl::MemoryBase::Type::buffer>>(
            std::move(propertiesVec), flags, host_ptr, size);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return buffer;
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

CL_API_ENTRY cl_mem CL_API_CALL
clCreateSubBuffer(cl_mem buffer,
                  cl_mem_flags flags,
                  cl_buffer_create_type buffer_create_type,
                  const void* buffer_create_info,
                  cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_1 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateImage(cl_context context,
              cl_mem_flags flags,
              const cl_image_format* image_format,
              const cl_image_desc* image_desc,
              void* host_ptr,
              cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_2 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreateImageWithProperties(cl_context context,
                            const cl_mem_properties* properties,
                            cl_mem_flags flags,
                            const cl_image_format* image_format,
                            const cl_image_desc* image_desc,
                            void* host_ptr,
                            cl_int* errcode_ret) CL_API_SUFFIX__VERSION_3_0 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_mem CL_API_CALL
clCreateImage2D(cl_context context,
                cl_mem_flags flags,
                const cl_image_format* image_format,
                size_t image_width,
                size_t image_height,
                size_t image_row_pitch,
                void* host_ptr,
                cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_mem CL_API_CALL
clCreateImage3D(cl_context context,
                cl_mem_flags flags,
                const cl_image_format* image_format,
                size_t image_width,
                size_t image_height,
                size_t image_depth,
                size_t image_row_pitch,
                size_t image_slice_pitch,
                void* host_ptr,
                cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetSupportedImageFormats(cl_context context,
                           cl_mem_flags flags,
                           cl_mem_object_type image_type,
                           cl_uint num_entries,
                           cl_image_format* image_formats,
                           cl_uint* num_image_formats) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetImageInfo(cl_mem image,
               cl_image_info param_name,
               size_t param_value_size,
               void* param_value,
               size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_mem CL_API_CALL
clCreatePipe(cl_context context,
             cl_mem_flags flags,
             cl_uint pipe_packet_size,
             cl_uint pipe_max_packets,
             const cl_pipe_properties* properties,
             cl_int* errcode_ret) CL_API_SUFFIX__VERSION_2_0 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetPipeInfo(cl_mem pipe,
              cl_pipe_info param_name,
              size_t param_value_size,
              void* param_value,
              size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!memobj || !memobj->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*memobj);
        memoryBaseInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!memobj || !memobj->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*memobj);
        memoryBaseInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetMemObjectDestructorCallback(cl_mem memobj,
                                 void (CL_CALLBACK* pfn_notify)(cl_mem memobj,
                                                                void* user_data),
                                 void* user_data) CL_API_SUFFIX__VERSION_1_1 {
    try {
        if (!memobj || !memobj->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }
        if (!pfn_notify) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*memobj);
        memoryBaseInternal.setDestructorCallback([pfn_notify, memobj, user_data] { pfn_notify(memobj, user_data); });
    } catch (const std::exception& e){
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetMemObjectInfo(cl_mem memobj,
                   cl_mem_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!memobj || !memobj->isValid()) {
            throw bud::cl::Except(CL_INVALID_MEM_OBJECT);
        }

        auto& memoryBaseInternal = static_cast<bud::cl::MemoryBase&>(*memobj);

        if (param_value) {
            memoryBaseInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = memoryBaseInternal.getInfoSize(param_name);
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY void* CL_API_CALL
clSVMAlloc(cl_context context,
           cl_svm_mem_flags flags,
           size_t size,
           cl_uint alignment) CL_API_SUFFIX__VERSION_2_0 {
    return nullptr;
}

CL_API_ENTRY void CL_API_CALL
clSVMFree(cl_context context,
          void* svm_pointer) CL_API_SUFFIX__VERSION_2_0 {
}
