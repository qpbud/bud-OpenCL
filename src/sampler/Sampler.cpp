#include <cstring>
#include "sampler/Sampler.hpp"

namespace bud::cl {

Sampler::Sampler(Context& context,
                 std::vector<cl_sampler_properties>&& properties,
                 cl_bool normalizedCoords,
                 cl_addressing_mode addressingMode,
                 cl_filter_mode filterMode)
    : Object<_cl_sampler>()
    , Scope(context)
    , H1DN<hal::Sampler>()
    , m_properties(std::move(properties))
    , m_normalizedCoords(normalizedCoords)
    , m_addressingMode(addressingMode)
    , m_filterMode(filterMode) {
    for (cl_uint i = 0; i < context.getDeviceCount(); i++) {
        append(context.getDevice(i), context[context.getDevice(i)]);
    }
}

size_t Sampler::getInfoSize(cl_sampler_info info) const {
    switch (info) {
        case CL_SAMPLER_REFERENCE_COUNT:
            return sizeof(cl_uint);
        case CL_SAMPLER_CONTEXT:
            return sizeof(cl_context);
        case CL_SAMPLER_NORMALIZED_COORDS:
            return sizeof(cl_bool);
        case CL_SAMPLER_ADDRESSING_MODE:
            return sizeof(cl_addressing_mode);
        case CL_SAMPLER_FILTER_MODE:
            return sizeof(cl_filter_mode);
        case CL_SAMPLER_PROPERTIES:
            return sizeof(cl_sampler_properties) * m_properties.size();
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void Sampler::getInfo(cl_sampler_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_SAMPLER_REFERENCE_COUNT:
            *static_cast<cl_uint*>(value) = count();
            break;
        case CL_SAMPLER_CONTEXT:
            *static_cast<cl_context*>(value) = m_context.get();
            break;
        case CL_SAMPLER_NORMALIZED_COORDS:
            *static_cast<cl_bool*>(value) = m_normalizedCoords;
            break;
        case CL_SAMPLER_ADDRESSING_MODE:
            *static_cast<cl_addressing_mode*>(value) = m_addressingMode;
            break;
        case CL_SAMPLER_FILTER_MODE:
            *static_cast<cl_filter_mode*>(value) = m_filterMode;
            break;
        case CL_SAMPLER_PROPERTIES:
            std::memcpy(value, m_properties.data(), sizeof(cl_sampler_properties) * m_properties.size());
            break;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

}
