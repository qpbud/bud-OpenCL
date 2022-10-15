#include <vector>
#include <CL/cl.h>
#include <gtest/gtest.h>

namespace qp::cl::tst {

TEST(context, clCreateContext0) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(nullptr), 0
    };
    cl_context context = clCreateContext(properties, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_PLATFORM);
}

TEST(context, clCreateContext1) {
    cl_int err;
    cl_context context = clCreateContext(nullptr, 0, nullptr, nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(context, clCreateContext2) {
    cl_device_id device = nullptr;
    cl_int err;
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_DEVICE);
}

TEST(context, clCreateContext3) {
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
            cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
            ASSERT_EQ(err, CL_SUCCESS);

            err = clReleaseContext(context);
            ASSERT_EQ(err, CL_SUCCESS);
        }
    }
}

TEST(context, clCreateContextFromType0) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(nullptr), 0
    };
    cl_context context = clCreateContextFromType(properties, CL_DEVICE_TYPE_ALL, nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_PLATFORM);
}

TEST(context, clCreateContextFromType1) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        cl_context context = clCreateContextFromType(nullptr, CL_DEVICE_TYPE_ALL, nullptr, nullptr, &err);
        ASSERT_EQ(err, CL_SUCCESS);

        err = clReleaseContext(context);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(context, clRetainContext0) {
    cl_int err = clRetainContext(nullptr);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(context, clRetainContext1) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint count;
    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    cl_uint i = 0;
    for (; i < 7; i++) {
        err = clRetainContext(context);
        ASSERT_EQ(err, CL_SUCCESS);

        err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(count, i + 2);
    }

    for (; i > 0; i--) {
        err = clReleaseContext(context);
        ASSERT_EQ(err, CL_SUCCESS);

        err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(count, i);
    }

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(context, clReleaseContext0) {
    cl_int err = clReleaseContext(nullptr);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(context, clReleaseContext1) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint count;
    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clRetainContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 2);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clRetainContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clRetainContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 3);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 2);

    err = clRetainContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clRetainContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 4);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(context, clGetContextInfo0) {
    size_t infoSize;
    cl_int err = clGetContextInfo(nullptr, CL_CONTEXT_REFERENCE_COUNT, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(context, clGetContextInfo1) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    size_t infoSize;
    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GT(infoSize, 1);

    cl_uint count;
    err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, infoSize - 1, &count, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(context, clGetContextInfo2) {
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
            cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
            ASSERT_EQ(err, CL_SUCCESS);

            size_t infoSize;
            err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_uint));

            cl_uint count;
            err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, infoSize, &count, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(count, 1);

            err = clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_uint));

            cl_uint numDevicesInContext;
            err = clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, infoSize, &numDevicesInContext, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(numDevicesInContext, 1);

            err = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_device_id));

            cl_device_id devicesInContext;
            err = clGetContextInfo(context, CL_CONTEXT_DEVICES, infoSize, &devicesInContext, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(devicesInContext, device);

            err = clGetContextInfo(context, CL_CONTEXT_PROPERTIES, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, 0);

            err = clReleaseContext(context);
            ASSERT_EQ(err, CL_SUCCESS);
        }
    }
}

static void callbackAdd(cl_context context, void* userData) {
    if (userData) {
        *static_cast<int*>(userData) += 4;
    }
}

static void callbackMul(cl_context context, void* userData) {
    if (userData) {
        *static_cast<int*>(userData) *= 7;
    }
}

TEST(context, clSetContextDestructorCallback0) {
    cl_int err = clSetContextDestructorCallback(nullptr, callbackAdd, nullptr);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(context, clSetContextDestructorCallback1) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clSetContextDestructorCallback(context, nullptr, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(context, clSetContextDestructorCallback2) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint numDevices;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numDevices, 1);

    std::vector<cl_device_id> devices(numDevices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_context context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    int value = 0;

    err = clSetContextDestructorCallback(context, callbackMul, &value);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clSetContextDestructorCallback(context, callbackAdd, &value);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);

    ASSERT_EQ(value, 28);
}

}
