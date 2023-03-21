#pragma once

#include "device/hal/Buffer.hpp"

namespace bud::cl::backend::vulkan {

class Buffer : public hal::Buffer {
public:
    Buffer(bool isImport, void* hostPtr, size_t size)
        : hal::Buffer(isImport, hostPtr, size) {}
    cl_int read(size_t offset, size_t size, void* ptr) override {
        return CL_SUCCESS;
    }
    cl_int write(size_t offset, size_t size, const void* ptr) override {
        return CL_SUCCESS;
    }
};

}
