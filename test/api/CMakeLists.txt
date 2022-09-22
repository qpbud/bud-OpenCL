set(CMAKE_CXX_STANDARD 17)

set(API_TEST_SOURCE PlatformTest.cpp
                    DeviceTest.cpp)

add_executable(ApiTest ${API_TEST_SOURCE})
target_include_directories(ApiTest PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
                                           ${CMAKE_SOURCE_DIR}/third_party/OpenCL-Headers)
target_link_libraries(ApiTest PRIVATE $<TARGET_OBJECTS:gtest>
                                      $<TARGET_OBJECTS:gtest_main>
                                      $<TARGET_OBJECTS:OpenCL_QP>)
target_compile_definitions(ApiTest PRIVATE CL_TARGET_OPENCL_VERSION=300)

add_test(NAME ApiTest COMMAND ApiTest)
