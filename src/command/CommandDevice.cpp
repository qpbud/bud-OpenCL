#include "command/CommandDevice.hpp"

namespace qp::cl {

Command<CommandBase::Type::device>::Command(Context& context, Device& device)
    : CommandBase(CommandBase::Type::device)
    , H1D1<detail::Command>(device, context[device]) {
}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::waitEvents)>,
    std::vector<boost::intrusive_ptr<EventBase>>&& events) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::setEvent)>,
    boost::intrusive_ptr<EventBase>&& event) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBuffer)>,
    Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBufferRect)>,
    Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion) {}

}
