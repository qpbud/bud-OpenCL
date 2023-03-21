#include "command/CommandBase.hpp"

namespace bud::cl {

CommandBase::CommandBase(CommandBase::Type type)
    : m_type(type) {}

CommandBase::Type CommandBase::type() const {
    return m_type;
}

}
