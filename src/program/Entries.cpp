#include <vector>
#include <cstring>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "program/Program.hpp"

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource(cl_context context,
                          cl_uint count,
                          const char** strings,
                          const size_t* lengths,
                          cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (count == 0 || !strings) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        std::string source;
        for (cl_uint i = 0; i < count; i++) {
            if (!strings[i]) {
                throw bud::cl::Except(CL_INVALID_VALUE);
            }
            if (lengths && lengths[i] != 0) {
                source.append(strings[i], lengths[i]);
            } else {
                source.append(strings[i], std::strlen(strings[i]));
            }
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        cl_program program = &contextInternal.create<bud::cl::Program>(std::move(source));

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return program;
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

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithIL(cl_context context,
                      const void* il,
                      size_t length,
                      cl_int* errcode_ret) CL_API_SUFFIX__VERSION_2_1 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!il || length == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        std::vector<unsigned char> ilVec(length);
        std::memcpy(ilVec.data(), il, length);

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        cl_program program = &contextInternal.create<bud::cl::Program>(std::move(ilVec));

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return program;
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

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary(cl_context context,
                          cl_uint num_devices,
                          const cl_device_id* device_list,
                          const size_t* lengths,
                          const unsigned char** binaries,
                          cl_int* binary_status,
                          cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        if (!device_list || num_devices == 0) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }
        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        std::vector<cl_device_id> devicesVec;
        std::vector<std::vector<unsigned char>> binariesVec;
        for (cl_uint i = 0; i < num_devices; i++) {
            auto& deviceInternal = static_cast<bud::cl::Device&>(*device_list[i]);
            bool containsDevice = false;
            for (cl_uint j = 0; j < contextInternal.getDeviceCount(); j++) {
                if (&contextInternal.getDevice(j) == &deviceInternal) {
                    containsDevice = true;
                    break;
                }
            }
            if (!containsDevice) {
                throw bud::cl::Except(CL_INVALID_DEVICE);
            }
            devicesVec.push_back(device_list[i]);
            std::vector<unsigned char> binary(lengths[i]);
            std::memcpy(binary.data(), binaries[i], lengths[i]);
            binariesVec.push_back(std::move(binary));
        }

        cl_program program = &contextInternal.create<bud::cl::Program>(std::move(devicesVec), std::move(binariesVec));

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return program;
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

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBuiltInKernels(cl_context context,
                                  cl_uint num_devices,
                                  const cl_device_id* device_list,
                                  const char* kernel_names,
                                  cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_2 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!program || !program->isValid()) {
            throw bud::cl::Except(CL_INVALID_PROGRAM);
        }

        auto& programInternal = static_cast<bud::cl::Program&>(*program);
        programInternal.retain();
        return CL_SUCCESS;
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!program || !program->isValid()) {
            throw bud::cl::Except(CL_INVALID_PROGRAM);
        }

        auto& programInternal = static_cast<bud::cl::Program&>(*program);
        programInternal.release();
        return CL_SUCCESS;
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }
}

CL_API_ENTRY CL_API_PREFIX__VERSION_2_2_DEPRECATED cl_int CL_API_CALL
clSetProgramReleaseCallback(cl_program program,
                            void (CL_CALLBACK* pfn_notify)(cl_program program,
                                                           void* user_data),
                            void* user_data) CL_API_SUFFIX__VERSION_2_2_DEPRECATED {
    try {
        if (!program || !program->isValid()) {
            throw bud::cl::Except(CL_INVALID_PROGRAM);
        }

        if (!pfn_notify) {
            throw bud::cl::Except(CL_INVALID_VALUE);
        }

        auto& programInternal = static_cast<bud::cl::Program&>(*program);
        programInternal.setReleaseCallback([pfn_notify, program, user_data] { pfn_notify(program, user_data); });
        return CL_SUCCESS;
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }
}

CL_API_ENTRY cl_int CL_API_CALL
clSetProgramSpecializationConstant(cl_program program,
                                   cl_uint spec_id,
                                   size_t spec_size,
                                   const void* spec_value) CL_API_SUFFIX__VERSION_2_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clBuildProgram(cl_program program,
               cl_uint num_devices,
               const cl_device_id* device_list,
               const char* options,
               void (CL_CALLBACK* pfn_notify)(cl_program program,
                                              void* user_data),
               void* user_data) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_int CL_API_CALL
clCompileProgram(cl_program program,
                 cl_uint num_devices,
                 const cl_device_id* device_list,
                 const char* options,
                 cl_uint num_input_headers,
                 const cl_program* input_headers,
                 const char** header_include_names,
                 void (CL_CALLBACK* pfn_notify)(cl_program program,
                                                void* user_data),
                 void* user_data) CL_API_SUFFIX__VERSION_1_2 {
    return CL_INVALID_OPERATION;
}

CL_API_ENTRY cl_program CL_API_CALL
clLinkProgram(cl_context context,
              cl_uint num_devices,
              const cl_device_id* device_list,
              const char* options,
              cl_uint num_input_programs,
              const cl_program* input_programs,
              void (CL_CALLBACK* pfn_notify)(cl_program program,
                                             void* user_data),
              void* user_data,
              cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_2 {
    if (errcode_ret) {
        *errcode_ret = CL_INVALID_OPERATION;
    }
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadPlatformCompiler(cl_platform_id platform) CL_API_SUFFIX__VERSION_1_2 {
    try {
        if (!platform || !platform->isValid()) {
            throw bud::cl::Except(CL_INVALID_PLATFORM);
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clUnloadCompiler() CL_API_SUFFIX__VERSION_1_1_DEPRECATED {
    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetProgramInfo(cl_program program,
                 cl_program_info param_name,
                 size_t param_value_size,
                 void* param_value,
                 size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!program || !program->isValid()) {
            throw bud::cl::Except(CL_INVALID_PROGRAM);
        }

        auto& programInternal = static_cast<bud::cl::Program&>(*program);

        if (param_value) {
            programInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = programInternal.getInfoSize(param_name);
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
clGetProgramBuildInfo(cl_program program,
                      cl_device_id device,
                      cl_program_build_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    return CL_INVALID_OPERATION;
}
