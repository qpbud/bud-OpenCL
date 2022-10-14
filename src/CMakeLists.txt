set(CMAKE_CXX_STANDARD 17)

set(PLATFORM_SOURCE platform/Entries.cpp
                    platform/Platform.cpp)
set(DEVICE_SOURCE device/Entries.cpp
                  device/Device.cpp
                  device/backend/vulkan/Device.cpp
                  device/backend/vulkan/Context.cpp)
set(CONTEXT_SOURCE context/Entries.cpp
                   context/Context.cpp)
set(QUEUE_SOURCE queue/Entries.cpp
                 queue/QueueBase.cpp
                 queue/QueueHost.cpp)

add_library(OpenCL_QP SHARED ${PLATFORM_SOURCE}
                             ${DEVICE_SOURCE}
                             ${CONTEXT_SOURCE}
                             ${QUEUE_SOURCE})
target_include_directories(OpenCL_QP PRIVATE ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/smart_ptr/include
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/config/include
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/assert/include
                                             ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(OpenCL_QP PRIVATE CL_TARGET_OPENCL_VERSION=300)
