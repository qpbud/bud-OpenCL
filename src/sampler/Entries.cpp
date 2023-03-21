#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "common/Except.hpp"
#include "context/Context.hpp"
#include "sampler/Sampler.hpp"

CL_API_ENTRY cl_sampler CL_API_CALL
clCreateSamplerWithProperties(cl_context context,
                              const cl_sampler_properties* sampler_properties,
                              cl_int* errcode_ret) CL_API_SUFFIX__VERSION_2_0 {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        std::vector<cl_sampler_properties> propertiesVec;
        cl_bool normalizedCoords = true;
        cl_addressing_mode addressingMode = CL_ADDRESS_CLAMP;
        cl_filter_mode filterMode = CL_FILTER_NEAREST;
        if (sampler_properties) {
            while (sampler_properties && *sampler_properties != 0) {
                switch (*sampler_properties) {
                    case CL_SAMPLER_NORMALIZED_COORDS:
                        normalizedCoords = *(sampler_properties + 1);
                        break;
                    case CL_SAMPLER_ADDRESSING_MODE:
                        addressingMode = *(sampler_properties + 1);
                        break;
                    case CL_SAMPLER_FILTER_MODE:
                        filterMode = *(sampler_properties + 1);
                        break;
                    default:
                        throw bud::cl::Except(CL_INVALID_QUEUE_PROPERTIES);
                }
                propertiesVec.push_back(*sampler_properties);
                propertiesVec.push_back(*(sampler_properties + 1));
                sampler_properties += 2;
            }
            propertiesVec.push_back(0);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        cl_sampler sampler = &contextInternal.create<bud::cl::Sampler>(std::move(propertiesVec), normalizedCoords, addressingMode, filterMode);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return sampler;
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

CL_API_ENTRY CL_API_PREFIX__VERSION_1_2_DEPRECATED cl_sampler CL_API_CALL
clCreateSampler(cl_context context,
                cl_bool normalized_coords,
                cl_addressing_mode addressing_mode,
                cl_filter_mode filter_mode,
                cl_int* errcode_ret) CL_API_SUFFIX__VERSION_1_2_DEPRECATED {
    try {
        if (!context || !context->isValid()) {
            throw bud::cl::Except(CL_INVALID_CONTEXT);
        }

        auto& contextInternal = static_cast<bud::cl::Context&>(*context);
        cl_sampler sampler = &contextInternal.create<bud::cl::Sampler>(std::vector<cl_sampler_properties>(), normalized_coords, addressing_mode, filter_mode);

        if (errcode_ret) {
            *errcode_ret = CL_SUCCESS;
        }
        return sampler;
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
clRetainSampler(cl_sampler sampler) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!sampler || !sampler->isValid()) {
            throw bud::cl::Except(CL_INVALID_SAMPLER);
        }

        auto& samplerInternal = static_cast<bud::cl::Sampler&>(*sampler);
        samplerInternal.retain();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseSampler(cl_sampler sampler) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!sampler || !sampler->isValid()) {
            throw bud::cl::Except(CL_INVALID_SAMPLER);
        }

        auto& samplerInternal = static_cast<bud::cl::Sampler&>(*sampler);
        samplerInternal.release();
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetSamplerInfo(cl_sampler sampler,
                 cl_sampler_info param_name,
                 size_t param_value_size,
                 void* param_value,
                 size_t* param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 {
    try {
        if (!sampler || !sampler->isValid()) {
            throw bud::cl::Except(CL_INVALID_SAMPLER);
        }

        auto& samplerInternal = static_cast<bud::cl::Sampler&>(*sampler);

        if (param_value) {
            samplerInternal.getInfo(param_name, param_value_size, param_value);
        }
        if (param_value_size_ret) {
            *param_value_size_ret = samplerInternal.getInfoSize(param_name);
        }
    } catch (const std::exception& e) {
        if (auto except = dynamic_cast<const bud::cl::Except*>(&e); except) {
            return except->err();
        }
        return CL_OUT_OF_HOST_MEMORY;
    }

    return CL_SUCCESS;
}
