set(CMAKE_CXX_STANDARD 17)

set(PLATFORM_SOURCE platform/Entries.cpp
                    platform/Platform.cpp)
set(DEVICE_SOURCE device/Entries.cpp
                  device/Device.cpp)

add_library(OpenCL_QP SHARED ${PLATFORM_SOURCE}
                             ${DEVICE_SOURCE})
target_include_directories(OpenCL_QP PRIVATE ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers
                                             ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(OpenCL_QP PRIVATE CL_TARGET_OPENCL_VERSION=300)
