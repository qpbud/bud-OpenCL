#pragma once

#include "common/Khronos.hpp"

namespace qp::cl {

enum class ObjectMagic {
    platform = 0x1,
    device = 0x2,
    invalid = 0xffff
};

template<ObjectMagic m>
struct ObjectBase {
    cl_icd_dispatch* icd = nullptr;
    ObjectMagic magic = m;

    ~ObjectBase() {
        magic = ObjectMagic::invalid;
    }

    bool isValid() const noexcept {
        return magic == m;
    }
};

}

struct _cl_platform_id : public qp::cl::ObjectBase<qp::cl::ObjectMagic::platform> {};
struct _cl_device_id : public qp::cl::ObjectBase<qp::cl::ObjectMagic::device> {};

namespace qp::cl {

template<typename T> class Object;

template<> class Object<_cl_platform_id> : public _cl_platform_id {};
template<> class Object<_cl_device_id> : public _cl_device_id {};

}
