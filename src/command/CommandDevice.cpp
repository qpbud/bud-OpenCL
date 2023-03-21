#include "command/CommandDevice.hpp"

namespace bud::cl {

Command<CommandBase::Type::device>::Command(Context& context, Device& device)
    : CommandBase(CommandBase::Type::device)
    , H1D1<hal::Command>(device, context[device]) {
}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::waitEvents)>,
    std::vector<boost::intrusive_ptr<EventBase>>&& events) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::setEvent)>,
    boost::intrusive_ptr<EventBase>&& event) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBuffer)>,
    Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion) {
    static_cast<hal::Command&>(*this).copyBuffer(srcBuffer[m_device], dstBuffer[m_device], copyRegion);
}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBufferRect)>,
    Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::fillBuffer)>,
    Memory<MemoryBase::Type::buffer>& buffer, const void* pattern, size_t patternSize, size_t offset, size_t size) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyImage)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::fillImage)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyImageToBuffer)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBufferToImage)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmMemFill)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::ndrangeKernel)>) {}

void Command<CommandBase::Type::device>::append(
    std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::task)>) {}

}
