#include <vector>
#include <array>
#include <cstring>
#include <CL/cl.h>
#include <gtest/gtest.h>

namespace qp::cl::tst {

TEST(queue, clCreateCommandQueueWithProperties0) {
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

    cl_command_queue queue = clCreateCommandQueueWithProperties(nullptr, devices[0], nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(queue, clCreateCommandQueueWithProperties1) {
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

    cl_command_queue queue = clCreateCommandQueueWithProperties(context, nullptr, nullptr, &err);
    ASSERT_EQ(err, CL_INVALID_DEVICE);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clCreateCommandQueueWithProperties2) {
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

    cl_queue_properties properties[] = {
        CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0
    };
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, devices[0], properties, &err);
    ASSERT_EQ(err, CL_INVALID_QUEUE_PROPERTIES);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clCreateCommandQueueWithProperties3) {
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

    cl_queue_properties properties[] = {
        CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0
    };
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, devices[0], properties, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clCreateCommandQueue0) {
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

    cl_command_queue queue = clCreateCommandQueue(nullptr, devices[0], 0, &err);
    ASSERT_EQ(err, CL_INVALID_CONTEXT);
}

TEST(queue, clCreateCommandQueue1) {
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

    cl_command_queue queue = clCreateCommandQueue(context, nullptr, 0, &err);
    ASSERT_EQ(err, CL_INVALID_DEVICE);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clCreateCommandQueue2) {
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

    cl_command_queue queue = clCreateCommandQueue(context, devices[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
    ASSERT_EQ(err, CL_INVALID_QUEUE_PROPERTIES);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clCreateCommandQueue3) {
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

    cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clRetainCommandQueue0) {
    cl_int err = clRetainCommandQueue(nullptr);
    ASSERT_EQ(err, CL_INVALID_COMMAND_QUEUE);
}

TEST(queue, clRetainCommandQueue1) {
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

    cl_command_queue queue = clCreateCommandQueueWithProperties(context, devices[0], nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint count;
    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    cl_uint i = 0;
    for (; i < 7; i++) {
        err = clRetainCommandQueue(queue);
        ASSERT_EQ(err, CL_SUCCESS);

        err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(count, i + 2);
    }

    for (; i > 0; i--) {
        err = clReleaseCommandQueue(queue);
        ASSERT_EQ(err, CL_SUCCESS);

        err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(count, i);
    }

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clReleaseCommandQueue0) {
    cl_int err = clReleaseCommandQueue(nullptr);
    ASSERT_EQ(err, CL_INVALID_COMMAND_QUEUE);
}

TEST(queue, clReleaseCommandQueue1) {
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

    cl_command_queue queue = clCreateCommandQueueWithProperties(context, devices[0], nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    cl_uint count;
    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clRetainCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 2);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clRetainCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clRetainCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 3);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 2);

    err = clRetainCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clRetainCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 4);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);
    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, sizeof(count), &count, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_EQ(count, 1);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clGetCommandQueueInfo0) {
    size_t infoSize;
    cl_int err = clGetCommandQueueInfo(nullptr, CL_QUEUE_REFERENCE_COUNT, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_INVALID_COMMAND_QUEUE);
}

TEST(queue, clGetCommandQueueInfo1) {
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

    cl_command_queue queue = clCreateCommandQueueWithProperties(context, devices[0], nullptr, &err);
    ASSERT_EQ(err, CL_SUCCESS);

    size_t infoSize;
    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GT(infoSize, 1);

    cl_uint count;
    err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, infoSize - 1, &count, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);

    err = clReleaseCommandQueue(queue);
    ASSERT_EQ(err, CL_SUCCESS);

    err = clReleaseContext(context);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(queue, clGetCommandQueueInfo2) {
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

            cl_queue_properties properties[] = {
                CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0
            };
            cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, properties, &err);
            ASSERT_EQ(err, CL_SUCCESS);

            size_t infoSize;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_CONTEXT, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_context));

            cl_context queueContext;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_CONTEXT, infoSize, &queueContext, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(queueContext, context);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_context));

            cl_device_id queueDevice;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE, infoSize, &queueDevice, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(queueDevice, device);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_uint));

            cl_uint count;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT, infoSize, &count, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(count, 1);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_PROPERTIES, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_command_queue_properties));

            cl_command_queue_properties propertyBits;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_PROPERTIES, infoSize, &propertyBits, nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(propertyBits, CL_QUEUE_PROFILING_ENABLE);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_PROPERTIES_ARRAY, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(infoSize, sizeof(cl_queue_properties) * 3);

            std::array<cl_queue_properties, 3> queueProperties;
            err = clGetCommandQueueInfo(queue, CL_QUEUE_PROPERTIES_ARRAY, infoSize, queueProperties.data(), nullptr);
            ASSERT_EQ(err, CL_SUCCESS);
            ASSERT_EQ(std::memcmp(properties, queueProperties.data(), infoSize), 0);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_SIZE, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_INVALID_QUEUE_PROPERTIES);

            err = clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE_DEFAULT, 0, nullptr, &infoSize);
            ASSERT_EQ(err, CL_INVALID_QUEUE_PROPERTIES);

            err = clReleaseCommandQueue(queue);
            ASSERT_EQ(err, CL_SUCCESS);

            err = clReleaseContext(context);
            ASSERT_EQ(err, CL_SUCCESS);
        }
    }
}

}
