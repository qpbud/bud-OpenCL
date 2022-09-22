#include <vector>
#include <CL/cl.h>
#include <gtest/gtest.h>

namespace qp::cl::tst {

TEST(device, clGetDeviceIDs0) {
    cl_device_id device;
    cl_int err = clGetDeviceIDs(nullptr, CL_DEVICE_TYPE_ALL, 1, &device, nullptr);
    ASSERT_EQ(err, CL_INVALID_PLATFORM);
}

TEST(device, clGetDeviceIDs1) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, &device, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(device, clGetDeviceIDs2) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        cl_uint numDevices;
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_GE(numDevices, 1);

        std::vector<cl_device_id> devices(numDevices);
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(device, clGetDeviceInfo0) {
    size_t infoSize;
    cl_int err = clGetDeviceInfo(nullptr, CL_DEVICE_TYPE, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_INVALID_DEVICE);
}

TEST(device, clGetDeviceInfo1) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    size_t infoSize;
    err = clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GT(infoSize, 1);

    cl_device_type type;
    err = clGetDeviceInfo(device, CL_DEVICE_TYPE, infoSize - 1, &type, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(device, clGetDeviceInfo2) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        cl_uint numDevices;
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_GE(numDevices, 1);

        std::vector<cl_device_id> devices(numDevices);
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);

        for (auto device : devices) {
            size_t infoSize;
            err = clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_device_type));

            cl_device_type type;
            err = clGetDeviceInfo(device, CL_DEVICE_TYPE, infoSize, &type, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
        }
    }
}

}
