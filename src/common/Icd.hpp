#pragma once

#include "common/Khronos.hpp"

namespace bud::cl {

inline cl_icd_dispatch* getIcdDispatch() {
    static cl_icd_dispatch icdTable = {
        /* OpenCL 1.0 */
        clGetPlatformIDs,
        clGetPlatformInfo,
        clGetDeviceIDs,
        clGetDeviceInfo,
        clCreateContext,
        clCreateContextFromType,
        clRetainContext,
        clReleaseContext,
        clGetContextInfo,
        clCreateCommandQueue,
        clRetainCommandQueue,
        clReleaseCommandQueue,
        clGetCommandQueueInfo,
        clSetCommandQueueProperty,
        clCreateBuffer,
        clCreateImage2D,
        clCreateImage3D,
        clRetainMemObject,
        clReleaseMemObject,
        clGetSupportedImageFormats,
        clGetMemObjectInfo,
        clGetImageInfo,
        clCreateSampler,
        clRetainSampler,
        clReleaseSampler,
        clGetSamplerInfo,
        clCreateProgramWithSource,
        clCreateProgramWithBinary,
        clRetainProgram,
        clReleaseProgram,
        clBuildProgram,
        clUnloadCompiler,
        clGetProgramInfo,
        clGetProgramBuildInfo,
        clCreateKernel,
        clCreateKernelsInProgram,
        clRetainKernel,
        clReleaseKernel,
        clSetKernelArg,
        clGetKernelInfo,
        clGetKernelWorkGroupInfo,
        clWaitForEvents,
        clGetEventInfo,
        clRetainEvent,
        clReleaseEvent,
        clGetEventProfilingInfo,
        clFlush,
        clFinish,
        clEnqueueReadBuffer,
        clEnqueueWriteBuffer,
        clEnqueueCopyBuffer,
        clEnqueueReadImage,
        clEnqueueWriteImage,
        clEnqueueCopyImage,
        clEnqueueCopyImageToBuffer,
        clEnqueueCopyBufferToImage,
        clEnqueueMapBuffer,
        clEnqueueMapImage,
        clEnqueueUnmapMemObject,
        clEnqueueNDRangeKernel,
        clEnqueueTask,
        clEnqueueNativeKernel,
        clEnqueueMarker,
        clEnqueueWaitForEvents,
        clEnqueueBarrier,
        nullptr, // clGetExtensionFunctionAddress,
        nullptr, // clCreateFromGLBuffer,
        nullptr, // clCreateFromGLTexture2D,
        nullptr, // clCreateFromGLTexture3D,
        nullptr, // clCreateFromGLRenderbuffer,
        nullptr, // clGetGLObjectInfo,
        nullptr, // clGetGLTextureInfo,
        nullptr, // clEnqueueAcquireGLObjects,
        nullptr, // clEnqueueReleaseGLObjects,
        nullptr, // clGetGLContextInfoKHR,

        /* cl_khr_d3d10_sharing */
        nullptr, // clGetDeviceIDsFromD3D10KHR,
        nullptr, // clCreateFromD3D10BufferKHR,
        nullptr, // clCreateFromD3D10Texture2DKHR,
        nullptr, // clCreateFromD3D10Texture3DKHR,
        nullptr, // clEnqueueAcquireD3D10ObjectsKHR,
        nullptr, // clEnqueueReleaseD3D10ObjectsKHR,

        /* OpenCL 1.1 */
        clSetEventCallback,
        clCreateSubBuffer,
        clSetMemObjectDestructorCallback,
        clCreateUserEvent,
        clSetUserEventStatus,
        clEnqueueReadBufferRect,
        clEnqueueWriteBufferRect,
        clEnqueueCopyBufferRect,

        /* cl_ext_device_fission */
        nullptr, // clCreateSubDevicesEXT,
        nullptr, // clRetainDeviceEXT,
        nullptr, // clReleaseDeviceEXT,

        /* cl_khr_gl_event */
        nullptr, // clCreateEventFromGLsyncKHR,

        /* OpenCL 1.2 */
        nullptr, // clCreateSubDevices,
        nullptr, // clRetainDevice,
        nullptr, // clReleaseDevice,
        clCreateImage,
        clCreateProgramWithBuiltInKernels,
        clCompileProgram,
        clLinkProgram,
        clUnloadPlatformCompiler,
        clGetKernelArgInfo,
        clEnqueueFillBuffer,
        clEnqueueFillImage,
        clEnqueueMigrateMemObjects,
        clEnqueueMarkerWithWaitList,
        clEnqueueBarrierWithWaitList,
        nullptr, // clGetExtensionFunctionAddressForPlatform,
        nullptr, // clCreateFromGLTexture,

        /* cl_khr_d3d11_sharing */
        nullptr, // clGetDeviceIDsFromD3D11KHR,
        nullptr, // clCreateFromD3D11BufferKHR,
        nullptr, // clCreateFromD3D11Texture2DKHR,
        nullptr, // clCreateFromD3D11Texture3DKHR,
        nullptr, // clCreateFromDX9MediaSurfaceKHR,
        nullptr, // clEnqueueAcquireD3D11ObjectsKHR,
        nullptr, // clEnqueueReleaseD3D11ObjectsKHR,

        /* cl_khr_dx9_media_sharing */
        nullptr, // clGetDeviceIDsFromDX9MediaAdapterKHR,
        nullptr, // clEnqueueAcquireDX9MediaSurfacesKHR,
        nullptr, // clEnqueueReleaseDX9MediaSurfacesKHR,

        /* cl_khr_egl_image */
        nullptr, // clCreateFromEGLImageKHR,
        nullptr, // clEnqueueAcquireEGLObjectsKHR,
        nullptr, // clEnqueueReleaseEGLObjectsKHR,

        /* cl_khr_egl_event */
        nullptr, // clCreateEventFromEGLSyncKHR,

        /* OpenCL 2.0 */
        clCreateCommandQueueWithProperties,
        clCreatePipe,
        clGetPipeInfo,
        clSVMAlloc,
        clSVMFree,
        clEnqueueSVMFree,
        clEnqueueSVMMemcpy,
        clEnqueueSVMMemFill,
        clEnqueueSVMMap,
        clEnqueueSVMUnmap,
        clCreateSamplerWithProperties,
        clSetKernelArgSVMPointer,
        clSetKernelExecInfo,

        /* cl_khr_sub_groups */
        nullptr, // clGetKernelSubGroupInfoKHR,

        /* OpenCL 2.1 */
        clCloneKernel,
        clCreateProgramWithIL,
        clEnqueueSVMMigrateMem,
        nullptr, // clGetDeviceAndHostTimer,
        nullptr, // clGetHostTimer,
        clGetKernelSubGroupInfo,
        clSetDefaultDeviceCommandQueue,

        /* OpenCL 2.2 */
        clSetProgramReleaseCallback,
        clSetProgramSpecializationConstant,

        /* OpenCL 3.0 */
        clCreateBufferWithProperties,
        clCreateImageWithProperties,
        clSetContextDestructorCallback,
    };
    return &icdTable;
}

}