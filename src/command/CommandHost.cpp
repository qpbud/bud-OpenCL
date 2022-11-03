#include "command/CommandHost.hpp"

namespace qp::cl {

Command<CommandBase::Type::host>::Command()
    : CommandBase(CommandBase::Type::host) {
}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::waitEvents)>,
    std::vector<boost::intrusive_ptr<EventBase>>&& events) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::setEvent)>,
    boost::intrusive_ptr<EventBase>&& event) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::readBuffer)>,
    Memory<MemoryBase::Type::buffer>& buffer, size_t offset, size_t size, void* ptr) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::writeBuffer)>,
    Memory<MemoryBase::Type::buffer>& buffer, size_t offset, size_t size, const void* ptr) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::readBufferRect)>,
    Memory<MemoryBase::Type::buffer>& buffer, void* ptr, const CopyRegion& copyRegion) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::writeBufferRect)>,
    Memory<MemoryBase::Type::buffer>& buffer, const void* ptr, const CopyRegion& copyRegion) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::mapBuffer)>,
    Memory<MemoryBase::Type::buffer>& buffer, void* mappedBuffer, cl_map_flags flags, size_t offset, size_t size) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::readImage)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::writeImage)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::mapImage)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::unmapMemObject)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::migrateMemObjects)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmFree)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmMemcpy)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmMap)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmUnmap)>) {}

void Command<CommandBase::Type::host>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmMigrateMem)>) {}

}
