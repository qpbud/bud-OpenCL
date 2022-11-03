#include "memory/MemoryBase.hpp"

namespace qp::cl {

MemoryBase::MemoryBase(Context& context,
                       MemoryBase::Type type,
                       cl_mem_object_type clType,
                       std::vector<cl_mem_properties>&& properties,
                       cl_mem_flags flags,
                       void* hostPtr)
    : Object<_cl_mem>()
    , Scope(context)
    , m_type(type)
    , m_clType(clType)
    , m_properties(std::move(properties))
    , m_flags(flags)
    , m_hostPtr(hostPtr)
    , m_destructorCallbacks() {}

MemoryBase::~MemoryBase() {
    while (!m_destructorCallbacks.empty()) {
        m_destructorCallbacks.top()();
        m_destructorCallbacks.pop();
    }
}

size_t MemoryBase::getInfoSize(cl_mem_info info) const {
    switch (info) {
        case CL_MEM_TYPE:
            return sizeof(cl_mem_object_type);
        case CL_MEM_FLAGS:
            return sizeof(cl_mem_flags);
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

void MemoryBase::getInfo(cl_mem_info info, size_t size, void* value) const {
    if (size < getInfoSize(info)) {
        throw Except(CL_INVALID_VALUE);
    }

    switch (info) {
        case CL_MEM_TYPE:
            *static_cast<cl_mem_object_type*>(value) = m_clType;
            break;
        case CL_MEM_FLAGS:
            *static_cast<cl_mem_flags*>(value) = m_flags;
        default:
            throw Except(CL_INVALID_VALUE);
    }
}

MemoryBase::Type MemoryBase::type() const {
    return m_type;
}

bool MemoryBase::withFlag(cl_mem_flags flag) const {
    return m_flags & flag != 0;
}

void MemoryBase::setDestructorCallback(std::function<void()>&& destructorCallback) {
    m_destructorCallbacks.push(std::move(destructorCallback));
}

}
