//
// Created by laurentiu on 06.06.2020.
//

#include "open_cl_kernel.hpp"
#include "main_open_cl_program.hpp"

void OpenCLKernel::deleteKernel(cl_kernel kernel) {
    if (!kernel)
        return;
    clReleaseKernel(kernel);
}

OpenCLKernel::OpenCLKernel(const std::string& name) {
    int errorCode;
    auto kernelTemp = clCreateKernel(program().get(), name.c_str(), &errorCode);
    if (!kernelTemp) {
        throw KernelCreationException(errorCode);
    }
    size workGroupSizeTemp = 0;
    errorCode = clGetKernelWorkGroupInfo(kernelTemp,
            device().value(),
            CL_KERNEL_WORK_GROUP_SIZE,
            sizeof(size),
            &workGroupSizeTemp,
            nullptr);
    if (errorCode != CL_SUCCESS) {
        throw KernelCreationException(errorCode);
    }
    kernel = KernelPtr(kernelTemp, &deleteKernel);
    workGroupSize = workGroupSizeTemp;
}

void OpenCLKernel::addArgument(OpenCLBuffer& bufferRef) {
    auto bufferLocation = buffer(bufferRef).get();
    int errorCode = clSetKernelArg(kernel.get(),
            argumentsCount.value_or(-1) + 1,
            sizeof(cl_mem),
            static_cast<void*>(&bufferLocation));
    if (errorCode != CL_SUCCESS) {
        throw KernelArgumentException(errorCode);
    }
    argumentsCount = argumentsCount.value_or(-1) + 1;
}

void OpenCLKernel::operator()(size x, size y, size z) {
    size sizes[3];
    sizes[0] = x;
    unsigned int index = 1;
    if (y > 0) {
        sizes[index] = y;
        ++index;
    }
    if (z > 0) {
        sizes[index] = z;
        ++index;
    }
    auto errorCode = clEnqueueNDRangeKernel(commandQueue().get(),
            kernel.get(),
            index,
            nullptr,
            sizes,
            nullptr,
            0,
            nullptr,
            nullptr);
    if (errorCode != CL_SUCCESS) {
        throw KernelRunException(errorCode);
    }
}

size OpenCLKernel::getWorkGroupSize() {
    return workGroupSize;
}
