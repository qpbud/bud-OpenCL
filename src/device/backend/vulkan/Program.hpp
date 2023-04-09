#pragma once

#include "device/hal/Program.hpp"

namespace bud::cl::backend::vulkan {

class Program : public hal::Program {
public:
    Program(hal::Program::SourceVariant&& sourceVariant) : hal::Program(std::move(sourceVariant)) {}
};

}
