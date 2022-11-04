#pragma once

#include <string>
#include "common/Khronos.hpp"
#include "common/Object.hpp"
#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "context/Context.hpp"
#include "context/Scope.hpp"
#include "program/Program.hpp"

namespace qp::cl {

class Kernel : public Object<_cl_kernel>, public Scope, public H1DN<detail::Kernel> {
    boost::intrusive_ptr<Program> m_program;
    std::string m_entry;
public:
    Kernel(Context& context, Program& program, std::string&& entry);
    Kernel(Context& context, const Kernel& srcKernel);

    void setArg(cl_uint index, size_t size, const void* value);
    void setArgSvmPointer(cl_uint index, const void* value);
    size_t getInfoSize(cl_kernel_info info) const;
    void getInfo(cl_kernel_info info, size_t size, void* value) const;
};

}
