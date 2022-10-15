set(CMAKE_CXX_STANDARD 17)

set(API_TEST_SOURCE PlatformTest.cpp
                    DeviceTest.cpp
                    ContextTest.cpp
                    QueueTest.cpp)

add_executable(ApiTest ${API_TEST_SOURCE})
target_include_directories(ApiTest PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
                                           ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers)
target_link_libraries(ApiTest PRIVATE $<TARGET_OBJECTS:gtest>
                                      $<TARGET_OBJECTS:gtest_main>
                                      $<TARGET_OBJECTS:OpenCL_QP>)
target_compile_definitions(ApiTest PRIVATE CL_TARGET_OPENCL_VERSION=300
                                           CL_USE_DEPRECATED_OPENCL_1_0_APIS
                                           CL_USE_DEPRECATED_OPENCL_1_1_APIS
                                           CL_USE_DEPRECATED_OPENCL_1_2_APIS
                                           CL_USE_DEPRECATED_OPENCL_2_0_APIS
                                           CL_USE_DEPRECATED_OPENCL_2_1_APIS
                                           CL_USE_DEPRECATED_OPENCL_2_2_APIS)

add_test(NAME ApiTest COMMAND ApiTest)
