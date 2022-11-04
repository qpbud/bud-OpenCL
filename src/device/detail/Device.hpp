#pragma once

#include <memory>
#include "common/Khronos.hpp"
#include "common/Except.hpp"
#include "device/detail/Context.hpp"
#include "device/detail/HostQueue.hpp"
#include "device/detail/Buffer.hpp"
#include "device/detail/Command.hpp"
#include "device/detail/Event.hpp"
#include "device/detail/Image.hpp"
#include "device/detail/Pipe.hpp"
#include "device/detail/Sampler.hpp"
#include "device/detail/Program.hpp"
#include "device/detail/Kernel.hpp"

namespace qp::cl::detail {

class Device {
    cl_device_type m_type;

    virtual std::unique_ptr<Context> createContext() = 0;
    std::unique_ptr<HostQueue> createHostQueue(Context& context) {
        return context.createHostQueue();
    }
    std::unique_ptr<Buffer> createBuffer(Context& context) {
        return context.createBuffer();
    }
    std::unique_ptr<Command> createCommand(Context& context) {
        return context.createCommand();
    }
    std::unique_ptr<Event> createEvent(Context& context) {
        return context.createEvent();
    }
    std::unique_ptr<Image> createImage(Context& context) {
        return context.createImage();
    }
    std::unique_ptr<Pipe> createPipe(Context& context) {
        return context.createPipe();
    }
    std::unique_ptr<Sampler> createSampler(Context& context) {
        return context.createSampler();
    }
    std::unique_ptr<Program> createProgram(Context& context) {
        return context.createProgram();
    }
    std::unique_ptr<Kernel> createKernel(Context& context) {
        return context.createKernel();
    }
public:
    template<typename Detail> struct Creator {
        template<typename ... Args> std::unique_ptr<Detail> operator()(Device& device, Args&&... args);
    };

    Device(cl_device_type type)
        : m_type(type) {
    }

    virtual ~Device() = default;

    cl_device_type type() const {
        return m_type;
    }

    size_t getInfoSize(cl_device_info info) const {
        switch (info) {
            case CL_DEVICE_TYPE:
                return sizeof(cl_device_type);
            default:
                throw Except(CL_INVALID_VALUE);
        }
    }

    void getInfo(cl_device_info info, size_t size, void* value) const {
        if (size < getInfoSize(info)) {
            throw Except(CL_INVALID_VALUE);
        }

        switch (info) {
            case CL_DEVICE_TYPE:
                *static_cast<cl_device_type*>(value) = m_type;
                break;
            default:
                throw Except(CL_INVALID_VALUE);
        }
    }
};

#define CREATE_FUNCTION(Detail) create##Detail

#define GENERATE_CREATOR(Detail) \
    template<> \
    template<typename ... Args> \
    std::unique_ptr<Detail> Device::Creator<Detail>::operator()(Device& device, Args&&... args) { \
        return device.CREATE_FUNCTION(Detail)(std::forward<Args>(args)...); \
    }

GENERATE_CREATOR(Context)
GENERATE_CREATOR(HostQueue)
GENERATE_CREATOR(Buffer)
GENERATE_CREATOR(Command)
GENERATE_CREATOR(Event)
GENERATE_CREATOR(Image)
GENERATE_CREATOR(Pipe)
GENERATE_CREATOR(Sampler)
GENERATE_CREATOR(Program)
GENERATE_CREATOR(Kernel)

}
