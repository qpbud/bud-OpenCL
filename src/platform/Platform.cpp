#include "platform/Platform.hpp"

namespace qp::cl {

std::vector<Platform> Platform::s_platforms{};
std::once_flag Platform::s_initFlag{};

void Platform::init() {
    s_platforms.emplace_back();
}

cl_uint Platform::count() {
    return static_cast<cl_uint>(s_platforms.size());
}

Platform& Platform::get(cl_uint index) {
    return s_platforms[index];
}

Platform::Platform() {}

}
