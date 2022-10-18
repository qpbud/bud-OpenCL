#pragma once

#include "device/Device.hpp"
#include "device/H2D.hpp"
#include "context/Context.hpp"
#include "command/CommandBase.hpp"

namespace qp::cl {

template<>
class Command<CommandBase::Type::device> : public CommandBase, public H1D1<detail::Command> {
public:
    Command(Context& context, Device& device);
};

}
