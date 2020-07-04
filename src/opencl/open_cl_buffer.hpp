//
// Created by laurentiu on 06.06.2020.
//

#ifndef RAULTESTQT_OPEN_CL_BUFFER_HPP
#define RAULTESTQT_OPEN_CL_BUFFER_HPP

#include <CL/cl.h>
#include <memory>
#include "../model/types.hpp"
#include "../exceptions/open_cl_exceptions.hpp"
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
    template<typename T>
    explicit OpenCLBuffer(std::vector<T>& data,
            OpenCLBufferMode mode = OpenCLBufferMode::READ_WRITE,
            OpenCLBufferMemoryType type = OpenCLBufferMemoryType::DEFAULT);
    template<typename T>
    void read(std::vector<T>& data);
    friend class OpenCLBufferAccess;
private:
    static void deleteBuffer(cl_mem buffer);
    static long flags(OpenCLBufferMode mode, OpenCLBufferMemoryType type);

public:
    using BufferPtr = std::unique_ptr<_cl_mem, decltype(&deleteBuffer)>;

private:
    BufferPtr buffer = BufferPtr(nullptr, &deleteBuffer);
};

class OpenCLBufferAccess {
protected:
    OpenCLBuffer::BufferPtr& buffer(OpenCLBuffer& bufferRef);
};

template<typename T>
OpenCLBuffer::OpenCLBuffer(std::vector<T>& data,
        OpenCLBufferMode mode,
        OpenCLBufferMemoryType type) {
    int error;
    auto bufferTemp = clCreateBuffer(context().get(),
            flags(mode, type),
            data.size() * sizeof(T),
            type == OpenCLBufferMemoryType::DEFAULT || type == OpenCLBufferMemoryType::GUEST ?
                static_cast<void*>(nullptr) :
                reinterpret_cast<void*>(data.data()),
            &error);
    if (!bufferTemp)
        throw BufferCreationException(error);
    buffer = BufferPtr(bufferTemp, &deleteBuffer);
}

template<typename T>
void OpenCLBuffer::read(std::vector<T>& data) {
    auto errorCode = clEnqueueReadBuffer(commandQueue().get(),
                                         buffer.get(),
                                         true,
                                         0,
                                         data.size() * sizeof(T),
                                         data.data(),
                                         0,
                                         nullptr,
                                         nullptr);
    if (errorCode != CL_SUCCESS) {
        throw BufferReadException(errorCode);
    }
}


#endif //RAULTESTQT_OPEN_CL_BUFFER_HPP
