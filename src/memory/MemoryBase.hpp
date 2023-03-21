#pragma once

#include <vector>
#include <array>
#include <stack>
#include <functional>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace bud::cl {

class MemoryBase : public Object<_cl_mem>, public Scope {
public:
    enum class Type { buffer, image, pipe };
private:
    Type m_type;
    cl_mem_object_type m_clType;
    std::vector<cl_mem_properties> m_properties;
    cl_mem_flags m_flags;
    void* m_hostPtr;
    std::stack<std::function<void()>> m_destructorCallbacks;
public:
    struct Region {
        std::array<size_t, 3> origin;
        std::array<size_t, 3> region;
        size_t rowPitch;
        size_t slicePitch;
    };

    MemoryBase(Context& context,
               Type type,
               cl_mem_object_type clType,
               std::vector<cl_mem_properties>&& properties,
               cl_mem_flags flags,
               void* hostPtr);
    virtual ~MemoryBase();

    size_t getInfoSize(cl_mem_info info) const;
    void getInfo(cl_mem_info info, size_t size, void* value) const;

    Type type() const;
    bool withFlag(cl_mem_flags flag) const;
    void setDestructorCallback(std::function<void()>&& destructorCallback);
};

template<MemoryBase::Type type> class Memory;

}
