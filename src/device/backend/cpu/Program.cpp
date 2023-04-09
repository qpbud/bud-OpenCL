#include "common/Khronos.hpp"
#include "device/backend/cpu/Program.hpp"

namespace bud::cl::backend::cpu {

Program::Program(hal::Program::SourceVariant&& sourceVariant)
    : hal::Program(std::move(sourceVariant)) {}

}