#pragma once

#include "device/hal/Program.hpp"

namespace bud::cl::backend::cpu {

class Program : public hal::Program {
public:
    Program(hal::Program::SourceVariant&& sourceVariant);
};

}
