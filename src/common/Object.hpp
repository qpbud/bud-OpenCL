#pragma once

#include <type_traits>
#include <concepts>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "common/Khronos.hpp"
#include "common/Icd.hpp"

namespace bud::cl {

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
    cl_icd_dispatch* icd = getIcdDispatch();
    ObjectMagic magic = m;

    ~ObjectBase() {
        magic = ObjectMagic::invalid;
    }

    bool isValid() const noexcept {
        return magic == m;
    }
};

}

struct _cl_platform_id : public bud::cl::ObjectBase<bud::cl::ObjectMagic::platform> {};
struct _cl_device_id : public bud::cl::ObjectBase<bud::cl::ObjectMagic::device> {};
struct _cl_context : public bud::cl::ObjectBase<bud::cl::ObjectMagic::context> {};
struct _cl_command_queue: public bud::cl::ObjectBase<bud::cl::ObjectMagic::queue> {};
struct _cl_mem: public bud::cl::ObjectBase<bud::cl::ObjectMagic::memory> {};
struct _cl_event: public bud::cl::ObjectBase<bud::cl::ObjectMagic::event> {};
struct _cl_sampler: public bud::cl::ObjectBase<bud::cl::ObjectMagic::sampler> {};
struct _cl_program: public bud::cl::ObjectBase<bud::cl::ObjectMagic::program> {};
struct _cl_kernel: public bud::cl::ObjectBase<bud::cl::ObjectMagic::kernel> {};

namespace bud::cl {

template<typename T>
concept UnrefObject = std::same_as<T, _cl_platform_id> ||
                      std::same_as<T, _cl_device_id>;

template<typename T>
concept RefObject = std::same_as<T, _cl_context> ||
                    std::same_as<T, _cl_command_queue> ||
                    std::same_as<T, _cl_mem> ||
                    std::same_as<T, _cl_event> ||
                    std::same_as<T, _cl_sampler> ||
                    std::same_as<T, _cl_program> ||
                    std::same_as<T, _cl_kernel>;

template<typename T> class Object;

template<UnrefObject T> class Object<T> : public T {};

template<RefObject T> class Object<T> : public T, public boost::intrusive_ref_counter<Object<T>> {
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
        return static_cast<cl_uint>(this->use_count());
    }
};

}