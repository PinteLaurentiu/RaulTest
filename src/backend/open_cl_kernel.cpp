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
    kernel = KernelPtr(kernelTemp, &deleteKernel);
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

void OpenCLKernel::operator()(const std::vector<WorkloadSpecification>& specification) {
    std::vector<size> globalSize;
    std::vector<size> offset;
    std::vector<size> localSize;
    globalSize.reserve(specification.size());
    offset.reserve(specification.size());
    localSize.reserve(specification.size());
    for (auto& spec : specification) {
        globalSize.push_back(spec.globalSize);
        offset.push_back(spec.offset);
        localSize.push_back(spec.localSize);
    }
    auto errorCode = clEnqueueNDRangeKernel(commandQueue().get(),
            kernel.get(),
            static_cast<unsigned int>(specification.size()),
            offset.data(),
            globalSize.data(),
            localSize.data(),
            0,
            nullptr,
            nullptr);
    if (errorCode != CL_SUCCESS)
        throw KernelRunException(errorCode);
}
