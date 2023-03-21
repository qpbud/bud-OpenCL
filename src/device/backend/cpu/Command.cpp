#include "device/backend/cpu/Command.hpp"
#include "device/backend/cpu/Buffer.hpp"

namespace bud::cl::backend::cpu {

void Command::copyBuffer(hal::Buffer& srcBuffer, hal::Buffer& dstBuffer, const CopyRegion& region) {
    auto& srcCpuBuffer = dynamic_cast<Buffer&>(srcBuffer);
    auto& dstCpuBuffer = dynamic_cast<Buffer&>(dstBuffer);
    m_chain.chainInPlace([&srcCpuBuffer, &dstCpuBuffer, region](cl_int errcode) {
        size_t srcOffset = region.src.origin[2] * region.src.slicePitch
                         + region.src.origin[1] * region.src.rowPitch
                         + region.src.origin[0];
        size_t dstOffset = region.dst.origin[2] * region.dst.slicePitch
                         + region.dst.origin[1] * region.dst.rowPitch
                         + region.dst.origin[0];
        for (size_t i = 0; i < region.region[2]; i++) {
            size_t srcOffsetTmp = srcOffset;
            size_t dstOffsetTmp = dstOffset;
            for (size_t j = 0; j < region.region[1]; j++) {
                std::memcpy(static_cast<char*>(dstCpuBuffer.data()) + dstOffsetTmp,
                            static_cast<char*>(srcCpuBuffer.data()) + srcOffsetTmp,
                            region.region[0]);
                srcOffsetTmp += region.src.rowPitch;
                dstOffsetTmp += region.dst.rowPitch;
            }
            srcOffset += region.src.slicePitch;
            dstOffset += region.dst.slicePitch;
        }
        return CL_SUCCESS;
    });
}

}