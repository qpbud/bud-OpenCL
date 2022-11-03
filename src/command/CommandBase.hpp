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
        fillBuffer,
        mapBuffer,
        readImage,
        writeImage,
        copyImage,
        fillImage,
        copyImageToBuffer,
        copyBufferToImage,
        mapImage,
        unmapMemObject,
        migrateMemObjects,
        svmFree,
        svmMemcpy,
        svmMemFill,
        svmMap,
        svmUnmap,
        svmMigrateMem,
    };

    template<cl_command_type commandType, typename Enable = void> struct TypeMap {};

    CommandBase(Type type);
    virtual ~CommandBase() = default;

    Type type() const;
};

template<CommandBase::Type type> class Command;

#define COMMAND_TRAITS(command, t, v) \
    template<cl_command_type commandType> \
    struct CommandBase::TypeMap<commandType, std::enable_if_t<commandType == command>> { \
        using type = Command<Type::t>;; \
        static constexpr Category value = Category::v; \
    };

COMMAND_TRAITS(CL_COMMAND_READ_BUFFER, host, readBuffer)
COMMAND_TRAITS(CL_COMMAND_WRITE_BUFFER, host, writeBuffer)
COMMAND_TRAITS(CL_COMMAND_READ_BUFFER_RECT, host, readBufferRect)
COMMAND_TRAITS(CL_COMMAND_WRITE_BUFFER_RECT, host, writeBufferRect)
COMMAND_TRAITS(CL_COMMAND_COPY_BUFFER, device, copyBuffer)
COMMAND_TRAITS(CL_COMMAND_COPY_BUFFER_RECT, device, copyBufferRect)
COMMAND_TRAITS(CL_COMMAND_FILL_BUFFER, device, fillBuffer)
COMMAND_TRAITS(CL_COMMAND_MAP_BUFFER, host, mapBuffer)
COMMAND_TRAITS(CL_COMMAND_READ_IMAGE, host, readImage)
COMMAND_TRAITS(CL_COMMAND_WRITE_IMAGE, host, writeImage)
COMMAND_TRAITS(CL_COMMAND_COPY_IMAGE, device, copyImage)
COMMAND_TRAITS(CL_COMMAND_FILL_IMAGE, device, fillImage)
COMMAND_TRAITS(CL_COMMAND_COPY_IMAGE_TO_BUFFER, device, copyImageToBuffer)
COMMAND_TRAITS(CL_COMMAND_COPY_BUFFER_TO_IMAGE, device, copyBufferToImage)
COMMAND_TRAITS(CL_COMMAND_MAP_IMAGE, host, mapImage)
COMMAND_TRAITS(CL_COMMAND_UNMAP_MEM_OBJECT, host, unmapMemObject)
COMMAND_TRAITS(CL_COMMAND_MIGRATE_MEM_OBJECTS, host, migrateMemObjects)
COMMAND_TRAITS(CL_COMMAND_SVM_FREE, host, svmFree)
COMMAND_TRAITS(CL_COMMAND_SVM_MEMCPY, host, svmMemcpy)
COMMAND_TRAITS(CL_COMMAND_SVM_MEMFILL, device, svmMemFill)
COMMAND_TRAITS(CL_COMMAND_SVM_MAP, host, svmMap)
COMMAND_TRAITS(CL_COMMAND_SVM_UNMAP, host, svmUnmap)
COMMAND_TRAITS(CL_COMMAND_SVM_MIGRATE_MEM, host, svmMigrateMem)

}
