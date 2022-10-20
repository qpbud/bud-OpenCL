#pragma once

#include <type_traits>
#include <utility>
#include "common/Khronos.hpp"

namespace qp::cl {

class CommandBase {
public:
    enum class Type { host, device };
private:
    Type m_type;
public:
    enum class Category {
        waitEvents,
        setEvent,
        readBuffer,
        writeBuffer,
        readBufferRect,
        writeBufferRect,
        copyBuffer,
        copyBufferRect,
    };

    template<cl_command_type commandType, typename Enable = void> struct TypeMap {};

    CommandBase(Type type);
    virtual ~CommandBase() = default;

    Type type() const;
};

template<CommandBase::Type type> class Command;

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_READ_BUFFER>> {
    using type = Command<Type::host>;
    static constexpr Category value = Category::readBuffer;
};

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_WRITE_BUFFER>> {
    using type = Command<Type::host>;
    static constexpr Category value = Category::writeBuffer;
};

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_READ_BUFFER_RECT>> {
    using type = Command<Type::host>;
    static constexpr Category value = Category::readBufferRect;
};

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_WRITE_BUFFER_RECT>> {
    using type = Command<Type::host>;
    static constexpr Category value = Category::writeBufferRect;
};

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_COPY_BUFFER>> {
    using type = Command<Type::device>;
    static constexpr Category value = Category::copyBuffer;
};

template<cl_command_type commandType>
struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == CL_COMMAND_COPY_BUFFER_RECT>> {
    using type = Command<Type::device>;
    static constexpr Category value = Category::copyBufferRect;
};

}
