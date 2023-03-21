#pragma once

#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace bud::cl {

class Sampler : public Object<_cl_sampler>, public Scope, public H1DN<hal::Sampler> {
    std::vector<cl_sampler_properties> m_properties;
    cl_bool m_normalizedCoords;
    cl_addressing_mode m_addressingMode;
    cl_filter_mode m_filterMode;
public:
    Sampler(Context& context,
            std::vector<cl_sampler_properties>&& properties,
            cl_bool normalizedCoords,
            cl_addressing_mode addressingMode,
            cl_filter_mode filterMode);

    size_t getInfoSize(cl_sampler_info info) const;
    void getInfo(cl_sampler_info info, size_t size, void* value) const;
};

}
