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

}
