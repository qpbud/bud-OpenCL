#pragma once

#include <vector>
#include <string>
#include <functional>
#include <stack>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"

namespace bud::cl {

class Program : public Object<_cl_program>, public Scope, public H1DN<hal::Program> {
    std::vector<std::string> m_sources;
    std::vector<unsigned char> m_il;
    std::vector<cl_device_id> m_devices;
    std::vector<std::vector<unsigned char>> m_binaries;
    std::stack<std::function<void()>> m_releaseCallbacks;
public:
    Program(Context& context, std::vector<std::string>&& sources);
    Program(Context& context, std::vector<unsigned char>&& il);
    Program(Context& context, std::vector<cl_device_id>&& devices, std::vector<std::vector<unsigned char>>&& binaries);
    ~Program();

    size_t getInfoSize(cl_program_info info) const;
    void getInfo(cl_program_info info, size_t size, void* value) const;
    void setReleaseCallback(std::function<void()>&& releaseCallback);
};

}
