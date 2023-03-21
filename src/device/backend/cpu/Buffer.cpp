#include <cstring>
#include "device/backend/cpu/buffer.hpp"

namespace bud::cl::backend::cpu {

Buffer::Buffer(bool isImport, void* hostPtr, size_t size)
    : hal::Buffer(isImport, hostPtr, size)
    , m_data(nullptr) {
    if (isImport) {
        m_data = hostPtr;
    } else {
        m_data = new char[size];
        std::memcpy(m_data, hostPtr, size);
    }
}

Buffer::~Buffer() {
    if (!m_isImport) {
        delete m_data;
    }
}

cl_int Buffer::read(size_t offset, size_t size, void* ptr) {
    std::memcpy(ptr, static_cast<char*>(m_data) + offset, size);
    return CL_SUCCESS;
}

cl_int Buffer::write(size_t offset, size_t size, const void* ptr) {
    std::memcpy(static_cast<char*>(m_data) + offset, ptr, size);
    return CL_SUCCESS;
}

void* Buffer::data() {
    return m_data;
}

}