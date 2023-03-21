#pragma once

#include "common/Khronos.hpp"

namespace bud::cl::hal {

class Buffer {
protected:
    bool m_isImport;
    void* m_hostPtr;
    size_t m_size;
public:
    Buffer(bool isImport, void* hostPtr, size_t size)
        : m_isImport(isImport)
        , m_hostPtr(hostPtr)
        , m_size(size) {}
    virtual ~Buffer() = default;
    virtual cl_int read(size_t offset, size_t size, void* ptr) = 0;
    virtual cl_int write(size_t offset, size_t size, const void* ptr) = 0;
};

}
