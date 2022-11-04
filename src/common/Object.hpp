#pragma once

#include <type_traits>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "common/Khronos.hpp"

namespace qp::cl {

enum class ObjectMagic {
    platform = 0x1,
    device = 0x2,
    context = 0x4,
    queue = 0x6,
    memory = 0x7,
    event = 0x11,
    sampler = 0x12,
    program = 0x14,
    kernel = 0x16,
    invalid = 0xffff,
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
struct _cl_context : public qp::cl::ObjectBase<qp::cl::ObjectMagic::context> {};
struct _cl_command_queue: public qp::cl::ObjectBase<qp::cl::ObjectMagic::queue> {};
struct _cl_mem: public qp::cl::ObjectBase<qp::cl::ObjectMagic::memory> {};
struct _cl_event: public qp::cl::ObjectBase<qp::cl::ObjectMagic::event> {};
struct _cl_sampler: public qp::cl::ObjectBase<qp::cl::ObjectMagic::sampler> {};
struct _cl_program: public qp::cl::ObjectBase<qp::cl::ObjectMagic::program> {};
struct _cl_kernel: public qp::cl::ObjectBase<qp::cl::ObjectMagic::kernel> {};

namespace qp::cl {

template<typename T, typename Enable = void> class Object {};

template<typename T>
class Object<T,
             std::enable_if_t<std::is_same_v<T, _cl_platform_id> ||
                              std::is_same_v<T, _cl_device_id>>> : public T {};

template<typename T>
class Object<T, std::enable_if_t<std::is_same_v<T, _cl_context> ||
                                 std::is_same_v<T, _cl_command_queue> ||
                                 std::is_same_v<T, _cl_mem> ||
                                 std::is_same_v<T, _cl_event> ||
                                 std::is_same_v<T, _cl_sampler> ||
                                 std::is_same_v<T, _cl_program> ||
                                 std::is_same_v<T, _cl_kernel>>> : public T, public boost::intrusive_ref_counter<Object<T>> {
public:
    Object()
        : T()
        , boost::intrusive_ref_counter<Object<T>>() {
        boost::sp_adl_block::intrusive_ptr_add_ref(this);
    }

    virtual ~Object() = default;

    void retain() {
        boost::sp_adl_block::intrusive_ptr_add_ref(this);
    }

    void release() {
        boost::sp_adl_block::intrusive_ptr_release(this);
    }

    cl_uint count() const {
        return static_cast<cl_uint>(use_count());
    }
};

}
