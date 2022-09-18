set(CMAKE_CXX_STANDARD 17)

set(PLATFORM_SOURCE platform/Entries.cpp
                    platform/Platform.cpp)

add_library(OpenCL_QP SHARED ${PLATFORM_SOURCE})
target_include_directories(OpenCL_QP PRIVATE ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers
                                             ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(OpenCL_QP PRIVATE CL_TARGET_OPENCL_VERSION=300)
