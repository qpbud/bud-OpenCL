#pragma once

#include <vector>
#include <type_traits>
#include <memory>
#include <list>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include "device/H2D.hpp"
#include "queue/QueueBase.hpp"
#include "command/CommandBase.hpp"
#include "command/CommandHost.hpp"
#include "command/CommandDevice.hpp"
#include "event/EventBase.hpp"

namespace qp::cl {

template<>
class Queue<QueueBase::Type::host> : public QueueBase, public H1D1<detail::HostQueue> {
    bool m_enableProfiling;
    std::list<std::unique_ptr<Command<CommandBase::Type::host>>> m_hostCommandList;
    std::list<std::unique_ptr<Command<CommandBase::Type::device>>> m_deviceCommandList;
public:
    Queue(Context& context,
          Device& device,
          std::vector<cl_queue_properties>&& properties,
          cl_command_queue_properties propertyBits,
          bool enableProfiling);

    template<cl_command_type type, typename ... Args>
    void enqueueCommand(std::vector<boost::intrusive_ptr<EventBase>>&& waitList,
                        boost::intrusive_ptr<EventBase> toSetEvent,
                        Args&&... args) {
        using CommandType = typename CommandBase::TypeMap<type>::type;
        std::unique_ptr<CommandType> command;
        if constexpr (std::is_same_v<CommandType, Command<CommandBase::Type::host>>) {
            command = std::make_unique<CommandType>();
        } else {
            command = std::make_unique<CommandType>(*m_context, m_device);
        }
        command->template append<CommandBase::Category::waitEvents>(std::move(waitList));
        command->template append<CommandBase::TypeMap<type>::value>(std::forward<Args>(args)...);
        command->template append<CommandBase::Category::setEvent>(std::move(toSetEvent));
        if constexpr (std::is_same_v<CommandType, Command<CommandBase::Type::host>>) {
            m_hostCommandList.push_back(std::move(command));
        } else {
            m_deviceCommandList.push_back(std::move(command));
        }
    }

    void flush();
    void finish();
};

}
