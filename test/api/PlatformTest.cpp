#include <vector>
#include <CL/cl.h>
#include <gtest/gtest.h>

namespace qp::tst {

TEST(platform, clGetPlatformIDs0) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(0, &platform, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(platform, clGetPlatformIDs1) {
    cl_int err = clGetPlatformIDs(0, nullptr, nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(platform, clGetPlatformIDs2) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);
}

TEST(platform, clGetPlatformInfo0) {
    size_t infoSize;
    cl_int err = clGetPlatformInfo(nullptr, CL_PLATFORM_PROFILE, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_INVALID_PLATFORM);
}

TEST(platform, clGetPlatformInfo1) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    size_t infoSize;
    err = clGetPlatformInfo(platform, CL_DEVICE_TYPE, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(platform, clGetPlatformInfo2) {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    size_t infoSize;
    err = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, 0, nullptr, &infoSize);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GT(infoSize, 1);

    auto profile = std::make_unique<char[]>(infoSize - 1);
    err = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, infoSize - 1, profile.get(), nullptr);
    ASSERT_EQ(err, CL_INVALID_VALUE);
}

TEST(platform, clGetPlatformInfo3) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);

        auto profile = std::make_unique<char[]>(infoSize);
        err = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, infoSize, profile.get(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo4) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);

        auto version = std::make_unique<char[]>(infoSize);
        err = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, infoSize, version.get(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo5) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_NUMERIC_VERSION, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(infoSize, sizeof(cl_version));

        cl_version version;
        err = clGetPlatformInfo(platform, CL_PLATFORM_NUMERIC_VERSION, infoSize, &version, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo6) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);

        auto name = std::make_unique<char[]>(infoSize);
        err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, infoSize, name.get(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo7) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);

        auto vendor = std::make_unique<char[]>(infoSize);
        err = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, infoSize, vendor.get(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo8) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);

        auto extensions = std::make_unique<char[]>(infoSize);
        err = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, infoSize, extensions.get(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo9) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS_WITH_VERSION, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(infoSize % sizeof(cl_name_version), 0);

        std::vector<cl_name_version> extensions(infoSize / sizeof(cl_name_version));
        err = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS_WITH_VERSION, infoSize, extensions.data(), nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

TEST(platform, clGetPlatformInfo10) {
    cl_uint numPlatforms;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    ASSERT_EQ(err, CL_SUCCESS);
    ASSERT_GE(numPlatforms, 1);

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    ASSERT_EQ(err, CL_SUCCESS);

    for (auto platform : platforms) {
        size_t infoSize;
        err = clGetPlatformInfo(platform, CL_PLATFORM_HOST_TIMER_RESOLUTION, 0, nullptr, &infoSize);
        ASSERT_EQ(err, CL_SUCCESS);
        ASSERT_EQ(infoSize, sizeof(cl_ulong));

        cl_ulong hostTimerResolution;
        err = clGetPlatformInfo(platform, CL_PLATFORM_HOST_TIMER_RESOLUTION, infoSize, &hostTimerResolution, nullptr);
        ASSERT_EQ(err, CL_SUCCESS);
    }
}

}
