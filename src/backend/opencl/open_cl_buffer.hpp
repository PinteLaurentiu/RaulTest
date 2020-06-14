//
// Created by laurentiu on 06.06.2020.
//

#ifndef RAULTESTQT_OPEN_CL_BUFFER_HPP
#define RAULTESTQT_OPEN_CL_BUFFER_HPP

#include <CL/cl.h>
#include <memory>
#include "src/backend/model/types.hpp"
#include "src/backend/exceptions/open_cl_exceptions.hpp"
#include "open_cl_context.hpp"

enum class OpenCLBufferMode : byte {
    READ,
    WRITE,
    READ_WRITE
};

enum class OpenCLBufferMemoryType : byte {
    HOST,
    GUEST,
    DUPLICATE,
    DEFAULT
};

class OpenCLBuffer : OpenCLContextAccess {
public:
    size getSize();
    template<typename T>
    OpenCLBuffer(T* value = nullptr,
            size valueSize = sizeof(T),
            OpenCLBufferMode mode = OpenCLBufferMode::READ_WRITE,
            OpenCLBufferMemoryType type = OpenCLBufferMemoryType::DEFAULT);
    template<typename T>
    void read(T* value);
    friend class OpenCLBufferAccess;
private:
    static void deleteBuffer(cl_mem buffer);
    static long flags(OpenCLBufferMode mode, OpenCLBufferMemoryType type);

public:
    using BufferPtr = std::unique_ptr<_cl_mem, decltype(&deleteBuffer)>;

private:
    BufferPtr buffer = BufferPtr(nullptr, &deleteBuffer);
    size bufferSize;
};

class OpenCLBufferAccess {
protected:
    OpenCLBuffer::BufferPtr& buffer(OpenCLBuffer& bufferRef);
};

template<typename T>
OpenCLBuffer::OpenCLBuffer(
        T* value,
        size valueSize,
        OpenCLBufferMode mode,
        OpenCLBufferMemoryType type) {
    int error;
    auto bufferTemp = clCreateBuffer(context().get(),
            flags(mode, type),
            valueSize,
            reinterpret_cast<void*>(value),
            &error);
    if (!bufferTemp)
        throw BufferCreationException(error);
    buffer = BufferPtr(bufferTemp, &deleteBuffer);
    bufferSize = valueSize;
}

template<typename T>
void OpenCLBuffer::read(T* value) {
    auto errorCode = clEnqueueReadBuffer(commandQueue().get(),
            buffer.get(),
            true,
            0,
            bufferSize,
            value,
            0,
            nullptr,
            nullptr);
    if (errorCode != CL_SUCCESS) {
        throw BufferReadException(errorCode);
    }
}


#endif //RAULTESTQT_OPEN_CL_BUFFER_HPP
