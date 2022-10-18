#include "command/CommandDevice.hpp"

namespace qp::cl {

Command<CommandBase::Type::device>::Command(Context& context, Device& device)
    : CommandBase(CommandBase::Type::device)
    , H1D1<detail::Command>(device, context[device]) {
}

}
