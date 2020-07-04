//
// Created by laurentiu on 06.06.2020.
//

#ifndef RAULTESTQT_OPEN_CL_KERNEL_HPP
#define RAULTESTQT_OPEN_CL_KERNEL_HPP

#include <CL/cl.h>
#include <memory>
#include <functional>
#include "open_cl_buffer.hpp"
#include "main_open_cl_program.hpp"
#include "open_cl_context.hpp"
#include "../model/types.hpp"
#include "open_cl_event.hpp"

class OpenCLKernel : OpenCLContextAccess, MainOpenCLProgramAccess, OpenCLBufferAccess {
public:
    explicit OpenCLKernel(const std::string& name);
    void addArgument(OpenCLBuffer& bufferRef);
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    void addNumericArgument(T argument);
    OpenCLEvent operator()(size x, size y = 0, size z = 0);
    size getWorkGroupSize();
    void waitFor(OpenCLEvent other);
private:
    static void deleteKernel(cl_kernel kernel);

private:
    using KernelPtr = std::unique_ptr<_cl_kernel, decltype(&deleteKernel)>;

private:
    KernelPtr kernel = KernelPtr(nullptr, &deleteKernel);
    std::vector<OpenCLEvent> events;
    std::optional<signed char> argumentsCount;
    size workGroupSize;

};

template<typename T, typename>
void OpenCLKernel::addNumericArgument(T argument) {
    int errorCode = clSetKernelArg(kernel.get(),
                                   argumentsCount.value_or(-1) + 1,
                                   sizeof(T),
                                   static_cast<void*>(&argument));
    if (errorCode != CL_SUCCESS) {
        throw KernelArgumentException(errorCode);
    }
    argumentsCount = argumentsCount.value_or(-1) + 1;
}

#endif //RAULTESTQT_OPEN_CL_KERNEL_HPP
