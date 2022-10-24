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
            throw qp::cl::Except(CL_INVALID_CONTEXT);
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
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if ((flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_WRITE_ONLY != 0) ||
            (flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_READ_ONLY != 0) ||
            (flags & CL_MEM_WRITE_ONLY != 0 && flags & CL_MEM_READ_ONLY != 0)) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (size == 0) {
            throw qp::cl::Except(CL_INVALID_BUFFER_SIZE);
        }

        if (!host_ptr && (flags & CL_MEM_USE_HOST_PTR != 0 || flags & CL_MEM_COPY_HOST_PTR != 0)) {
            throw qp::cl::Except(CL_INVALID_HOST_PTR);
        }

        if (host_ptr && (flags & CL_MEM_USE_HOST_PTR == 0 && flags & CL_MEM_COPY_HOST_PTR == 0)) {
            throw qp::cl::Except(CL_INVALID_HOST_PTR);
        }

        auto& contextInternal = static_cast<qp::cl::Context&>(*context);

        cl_mem buffer = &contextInternal.create<qp::cl::Memory<qp::cl::MemoryBase::Type::buffer>>(
            std::vector<cl_mem_properties>(), flags, host_ptr, size);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return buffer;
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

CL_API_ENTRY cl_mem CL_API_CALL
clCreateBufferWithProperties(cl_context context,
                             const cl_mem_properties* properties,
                             cl_mem_flags flags,
                             size_t size,
                             void* host_ptr,
                             cl_int* errcode_ret) CL_API_SUFFIX__VERSION_3_0 {
    try {
        if (!context || !context->isValid()) {
            throw qp::cl::Except(CL_INVALID_CONTEXT);
        }

        std::vector<cl_mem_properties> propertiesVec;
        if (properties) {
            while (properties && *properties != 0) {
                switch (*properties) {
                    default:
                        throw qp::cl::Except(CL_INVALID_PROPERTY);
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
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if ((flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_WRITE_ONLY != 0) ||
            (flags & CL_MEM_READ_WRITE != 0 && flags & CL_MEM_READ_ONLY != 0) ||
            (flags & CL_MEM_WRITE_ONLY != 0 && flags & CL_MEM_READ_ONLY != 0)) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        if (size == 0) {
            throw qp::cl::Except(CL_INVALID_BUFFER_SIZE);
        }

        if (!host_ptr && (flags & CL_MEM_USE_HOST_PTR != 0 || flags & CL_MEM_COPY_HOST_PTR != 0)) {
            throw qp::cl::Except(CL_INVALID_HOST_PTR);
        }

        if (host_ptr && (flags & CL_MEM_USE_HOST_PTR == 0 && flags & CL_MEM_COPY_HOST_PTR == 0)) {
            throw qp::cl::Except(CL_INVALID_HOST_PTR);
        }

        auto& contextInternal = static_cast<qp::cl::Context&>(*context);

        cl_mem buffer = &contextInternal.create<qp::cl::Memory<qp::cl::MemoryBase::Type::buffer>>(
            std::move(propertiesVec), flags, host_ptr, size);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return buffer;
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
