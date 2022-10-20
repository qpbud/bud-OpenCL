#pragma once

#include <vector>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "common/Struct.hpp"
#include "memory/MemoryBuffer.hpp"
#include "command/CommandBase.hpp"
#include "event/EventBase.hpp"

namespace qp::cl {

template<>
class Command<CommandBase::Type::host> : public CommandBase {
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::waitEvents)>,
                std::vector<boost::intrusive_ptr<EventBase>>&& events);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::setEvent)>,
                boost::intrusive_ptr<EventBase>&& event);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::readBuffer)>,
                Memory<MemoryBase::Type::buffer>& buffer, size_t offset, size_t size, void* ptr);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::writeBuffer)>,
                Memory<MemoryBase::Type::buffer>& buffer, size_t offset, size_t size, const void* ptr);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::readBufferRect)>,
                Memory<MemoryBase::Type::buffer>& buffer, void* ptr, const CopyRegion& copyRegion);
    void append(std::in_place_index_t<static_cast<std::size_t>(CommandBase::Category::writeBufferRect)>,
                Memory<MemoryBase::Type::buffer>& buffer, const void* ptr, const CopyRegion& copyRegion);
public:
    Command();

    template<CommandBase::Category category, typename ... Args>
    void append(Args&&... args) {
        return append(std::in_place_index<static_cast<std::size_t>(category)>, std::forward<Args>(args)...);
    }
};

}
