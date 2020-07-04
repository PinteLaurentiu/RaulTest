//
// Created by laurentiu on 06.06.2020.
//

#include "open_cl_buffer.hpp"

long OpenCLBuffer::flags(OpenCLBufferMode mode, OpenCLBufferMemoryType type) {
    long result = 0;
    switch (mode) {
        case OpenCLBufferMode::READ:
            result |= CL_MEM_READ_ONLY;
            break;
        case OpenCLBufferMode::WRITE:
            result |= CL_MEM_WRITE_ONLY;
            break;
        case OpenCLBufferMode::READ_WRITE:
            result |= CL_MEM_READ_WRITE;
            break;
    }
    switch (type) {
        case OpenCLBufferMemoryType::HOST:
            result |= CL_MEM_USE_HOST_PTR;
            break;
        case OpenCLBufferMemoryType::GUEST:
            result |= CL_MEM_ALLOC_HOST_PTR;
            break;
        case OpenCLBufferMemoryType::DUPLICATE:
            result |= CL_MEM_COPY_HOST_PTR;
            break;
        case OpenCLBufferMemoryType::DEFAULT:
            break;
    }
    return result;
}

void OpenCLBuffer::deleteBuffer(cl_mem buffer) {
    if (!buffer)
        return;
    clReleaseMemObject(buffer);
}

OpenCLBuffer::BufferPtr& OpenCLBufferAccess::buffer(OpenCLBuffer& bufferRef) {
    return bufferRef.buffer;
}
