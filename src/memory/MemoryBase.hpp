#pragma once

#include <vector>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace qp::cl {

class MemoryBase : public Object<_cl_mem>, public Scope {
public:
    enum class Type { buffer, image, pipe };
private:
    Type m_type;
    std::vector<cl_mem_properties> m_properties;
    cl_mem_flags m_flags;
    void* m_hostPtr;
public:
    MemoryBase(Context& context,
               Type type,
               std::vector<cl_mem_properties>&& properties,
               cl_mem_flags flags,
               void* hostPtr);
    virtual ~MemoryBase() = default;

    size_t getInfoSize(cl_mem_info info) const;
    void getInfo(cl_mem_info info, size_t size, void* value) const;

    Type type() const;
};

template<MemoryBase::Type type> class Memory;

}
