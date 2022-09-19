cmake_minimum_required(VERSION 3.21)

project(qp-OpenCL
        VERSION 0.1
        LANGUAGES CXX)

add_subdirectory(src)

enable_testing()
add_subdirectory(third_party/googletest)

add_subdirectory(test)
