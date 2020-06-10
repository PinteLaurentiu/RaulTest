//
// Created by laurentiu on 06.06.2020.
//

#ifndef RAULTESTQT_OPEN_CL_KERNEL_HPP
#define RAULTESTQT_OPEN_CL_KERNEL_HPP

#include <CL/cl.h>
#include <memory>
#include <functional>
#include "open_cl_exceptions.hpp"
#include "types.hpp"
#include "open_cl_buffer.hpp"
#include "main_open_cl_program.hpp"
#include "open_cl_context.hpp"

struct WorkloadSpecification {
    WorkloadSpecification(size globalSize, size offset, size localSize);
    size globalSize;
    size offset;
    size localSize;
};

class OpenCLKernel : OpenCLContextAccess, MainOpenCLProgramAccess, OpenCLBufferAccess {
public:
    explicit OpenCLKernel(const std::string& name);
    void addArgument(OpenCLBuffer& bufferRef);
    void operator()(size globalSize);
    void operator()(const std::vector<WorkloadSpecification>& specification);
    size getWorkGroupSize();
private:
    static void deleteKernel(cl_kernel kernel);

private:
    using KernelPtr = std::unique_ptr<_cl_kernel, decltype(&deleteKernel)>;

private:
    KernelPtr kernel = KernelPtr(nullptr, &deleteKernel);
    std::optional<signed char> argumentsCount;
    size workGroupSize;
};


#endif //RAULTESTQT_OPEN_CL_KERNEL_HPP
