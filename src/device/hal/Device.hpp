#pragma once

#include <memory>
#include "common/Khronos.hpp"
#include "common/Except.hpp"
#include "device/hal/Context.hpp"
#include "device/hal/HostQueue.hpp"
#include "device/hal/Buffer.hpp"
#include "device/hal/Command.hpp"
#include "device/hal/Event.hpp"
#include "device/hal/Image.hpp"
#include "device/hal/Pipe.hpp"
#include "device/hal/Sampler.hpp"
#include "device/hal/Program.hpp"
#include "device/hal/Kernel.hpp"

namespace bud::cl::hal {

class Device {
    cl_device_type m_type;

    virtual std::unique_ptr<Context> createContext() = 0;
    std::unique_ptr<HostQueue> createHostQueue(Context& context) {
        return context.createHostQueue();
    }
    std::unique_ptr<Buffer> createBuffer(Context& context, bool isImport, void* hostPtr, size_t size) {
        return context.createBuffer(isImport, hostPtr, size);
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
    template<typename Hal> struct Creator {
        template<typename ... Args> std::unique_ptr<Hal> operator()(Device& device, Args&&... args);
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

#define CREATE_FUNCTION(Hal) create##Hal

#define GENERATE_CREATOR(Hal) \
    template<> \
    template<typename ... Args> \
    std::unique_ptr<Hal> Device::Creator<Hal>::operator()(Device& device, Args&&... args) { \
        return device.CREATE_FUNCTION(Hal)(std::forward<Args>(args)...); \
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
