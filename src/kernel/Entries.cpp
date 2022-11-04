#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "program/Program.hpp"
#include "kernel/Kernel.hpp"

CL_API_ENTRY cl_kernel CL_API_CALL
clCreateKernel(cl_program program,
               const char* kernel_name,
               cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!program || !program->isValid()) {
            throw qp::cl::Except(CL_INVALID_PROGRAM);
        }

        if (!kernel_name) {
            throw qp::cl::Except(CL_INVALID_VALUE);
        }

        auto& programInternal = static_cast<qp::cl::Program&>(*program);
        auto& contextInternal = programInternal.getContext();
        std::string entry(kernel_name);
        cl_kernel kernel = &contextInternal.create<qp::cl::Kernel>(programInternal, std::move(entry));

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return kernel;
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
clCreateKernelsInProgram(cl_program program,
                         cl_uint num_kernels,
                         cl_kernel* kernels,
                         cl_uint* num_kernels_ret) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainKernel(cl_kernel kernel) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!kernel || !kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*kernel);
        kernelInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseKernel(cl_kernel kernel) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!kernel || !kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*kernel);
        kernelInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArg(cl_kernel kernel,
               cl_uint arg_index,
               size_t arg_size,
               const void* arg_value) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!kernel || !kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*kernel);
        kernelInternal.setArg(arg_index, arg_size, arg_value);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArgSVMPointer(cl_kernel kernel,
                         cl_uint arg_index,
                         const void* arg_value) CL_API_SUFFIX__VERSION_2_0 {
    try {
        if (!kernel || !kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*kernel);
        kernelInternal.setArgSvmPointer(arg_index, arg_value);
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const qp::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetKernelExecInfo(cl_kernel kernel,
                    cl_kernel_exec_info param_name,
                    size_t param_value_size,
                    const void* param_value) CL_API_SUFFIX__VERSION_2_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_kernel CL_API_CALL
clCloneKernel(cl_kernel source_kernel,
              cl_int* errcode_ret) CL_API_SUFFIX__VERSION_2_1 {
    try {
        if (!source_kernel || !source_kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*source_kernel);
        auto& contextInternal = kernelInternal.getContext();
        cl_kernel kernel = &contextInternal.create<qp::cl::Kernel>(kernelInternal);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return kernel;
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
clGetKernelInfo(cl_kernel kernel,
                cl_kernel_info param_name,
                size_t param_value_size,
                void* param_value,
                size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!kernel || !kernel->isValid()) {
            throw qp::cl::Except(CL_INVALID_KERNEL);
        }

        auto& kernelInternal = static_cast<qp::cl::Kernel&>(*kernel);

        if (param_value) {
            kernelInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = kernelInternal.getInfoSize(param_name);
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
clGetKernelWorkGroupInfo(cl_kernel kernel,
                         cl_device_id device,
                         cl_kernel_work_group_info param_name,
                         size_t param_value_size,
                         void* param_value,
                         size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelSubGroupInfo(cl_kernel kernel,
                        cl_device_id device,
                        cl_kernel_sub_group_info param_name,
                        size_t input_value_size,
                        const void* input_value,
                        size_t param_value_size,
                        void* param_value,
                        size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_2_1 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelArgInfo(cl_kernel kernel,
                   cl_uint arg_indx,
                   cl_kernel_arg_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}
