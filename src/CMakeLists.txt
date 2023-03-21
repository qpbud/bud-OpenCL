set(CMAKE_CXX_STANDARD 17)

set(PLATFORM_SOURCE platform/Entries.cpp
                    platform/Platform.cpp)
set(DEVICE_SOURCE device/Entries.cpp
                  device/Device.cpp)
set(CPU_BACKEND_SOURCE device/backend/cpu/Buffer.cpp
                       device/backend/cpu/Command.cpp
                       device/backend/cpu/Device.cpp
                       device/backend/cpu/Context.cpp)
set(VULKAN_BACKEND_SOURCE device/backend/vulkan/Device.cpp
                          device/backend/vulkan/Context.cpp)
set(CONTEXT_SOURCE context/Entries.cpp
                   context/Context.cpp)
set(QUEUE_SOURCE queue/Entries.cpp
                 queue/QueueBase.cpp
                 queue/QueueHost.cpp)
set(MEMORY_SOURCE memory/Entries.cpp
                  memory/MemoryBase.cpp
                  memory/MemoryBuffer.cpp
                  memory/MemoryImage.cpp
                  memory/MemoryPipe.cpp)
set(COMMAND_SOURCE command/Entries.cpp
                   command/CommandBase.cpp
                   command/CommandHost.cpp
                   command/CommandDevice.cpp)
set(EVENT_SOURCE event/Entries.cpp
                 event/EventBase.cpp
                 event/EventHost.cpp
                 event/EventDevice.cpp)
set(SAMPLER_SOURCE sampler/Entries.cpp
                   sampler/Sampler.cpp)
set(PROGRAM_SOURCE program/Entries.cpp
                   program/Program.cpp)
set(KERNEL_SOURCE kernel/Entries.cpp
                  kernel/Kernel.cpp)

add_library(OpenCL_BUD SHARED ${PLATFORM_SOURCE}
                             ${DEVICE_SOURCE}
                             ${CPU_BACKEND_SOURCE}
                             ${VULKAN_BACKEND_SOURCE}
                             ${CONTEXT_SOURCE}
                             ${QUEUE_SOURCE}
                             ${MEMORY_SOURCE}
                             ${COMMAND_SOURCE}
                             ${EVENT_SOURCE}
                             ${SAMPLER_SOURCE}
                             ${PROGRAM_SOURCE}
                             ${KERNEL_SOURCE})
target_include_directories(OpenCL_BUD PRIVATE ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/smart_ptr/include
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/config/include
                                             ${CMAKE_SOURCE_DIR}/third_party/boost/libs/assert/include
                                             ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(OpenCL_BUD PRIVATE CL_TARGET_OPENCL_VERSION=300
                                             CL_USE_DEPRECATED_OPENCL_1_0_APIS
                                             CL_USE_DEPRECATED_OPENCL_1_1_APIS
                                             CL_USE_DEPRECATED_OPENCL_1_2_APIS
                                             CL_USE_DEPRECATED_OPENCL_2_0_APIS
                                             CL_USE_DEPRECATED_OPENCL_2_1_APIS
                                             CL_USE_DEPRECATED_OPENCL_2_2_APIS)
