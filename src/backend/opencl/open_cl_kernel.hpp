//
// Created by laurentiu on 06.06.2020.
//

#ifndef RAULTESTQT_OPEN_CL_KERNEL_HPP
#define RAULTESTQT_OPEN_CL_KERNEL_HPP

#include <CL/cl.h>
#include <memory>
#include <functional>
#include "src/backend/exceptions/open_cl_exceptions.hpp"
#include "src/backend/model/types.hpp"
#include "open_cl_buffer.hpp"
#include "main_open_cl_program.hpp"
#include "open_cl_context.hpp"

class OpenCLKernel : OpenCLContextAccess, MainOpenCLProgramAccess, OpenCLBufferAccess {
public:
    explicit OpenCLKernel(const std::string& name);
    void addArgument(OpenCLBuffer& bufferRef);
    void operator()(size x, size y = 0, size z = 0);
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
