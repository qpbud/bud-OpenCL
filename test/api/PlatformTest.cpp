#include <CL/cl.h>
#include <gtest/gtest.h>

namespace qp::tst {

TEST(cl_platform, clGetPlatformIDs0) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(0, &platform, nullptr);
    EXPECT_EQ(err, CL_INVALID_VALUE);
}

TEST(cl_platform, clGetPlatformIDs1) {
    cl_int err = clGetPlatformIDs(0, nullptr, nullptr);
    EXPECT_EQ(err, CL_INVALID_VALUE);
}

TEST(cl_platform, clGetPlatformIDs2) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    EXPECT_EQ(err, CL_SUCCESS);
    EXPECT_GE(numPlatforms, 1);
    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    EXPECT_EQ(err, CL_SUCCESS);
}

}
