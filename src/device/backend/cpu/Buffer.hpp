#pragma once

#include "device/hal/Buffer.hpp"

namespace bud::cl::backend::cpu {

class Buffer : public hal::Buffer {
    void* m_data;
public:
    Buffer(bool isImport, void* hostPtr, size_t size);
    ~Buffer();
    cl_int read(size_t offset, size_t size, void* ptr) override;
    cl_int write(size_t offset, size_t size, const void* ptr) override;
    void* data();
};

}
