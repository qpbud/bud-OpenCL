#pragma once

#include <vector>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "common/Struct.hpp"
#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "context/Context.hpp"
#include "memory/MemoryBase.hpp"
#include "memory/MemoryBuffer.hpp"
#include "command/CommandBase.hpp"
#include "event/EventBase.hpp"

namespace qp::cl {

template<>
class Command<CommandBase::Type::device> : public CommandBase, public H1D1<detail::Command> {
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::waitEvents)>,
                std::vector<boost::intrusive_ptr<EventBase>>&& events);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::setEvent)>,
                boost::intrusive_ptr<EventBase>&& event);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBuffer)>,
                Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBufferRect)>,
                Memory<MemoryBase::Type::buffer>& srcBuffer, Memory<MemoryBase::Type::buffer>& dstBuffer, const CopyRegion& copyRegion);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::fillBuffer)>,
                Memory<MemoryBase::Type::buffer>& buffer, const void* pattern, size_t patternSize, size_t offset, size_t size);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyImage)>);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::fillImage)>);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyImageToBuffer)>);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::copyBufferToImage)>);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::svmMemFill)>);
public:
    Command(Context& context, Device& device);

    template<CommandBase::Category category, typename ... Args>
    void append(Args&&... args) {
        return append(std::in_place_index<static_cast<std::size_t>(category)>, std::forward<Args>(args)...);
    }
};

}
