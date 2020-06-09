//
// Created by laurentiu on 31.05.2020.
//

#include <sstream>
#include "open_cl_exceptions.hpp"

OpenCLException::OpenCLException(const std::string& reason) : runtime_error(reason) {}

SourceIOException::SourceIOException(const std::string& path, const std::string& reason) :
    OpenCLException(createMessage(path, reason)) {}

std::string SourceIOException::createMessage(const std::string& path, const std::string& reason) {
    std::ostringstream output;
    output << "Could not read source file " << path << " !";
    if (!reason.empty())
        output << " Reason: " << reason;
    return output.str();
}

ProgramCompilationException::ProgramCompilationException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string ProgramCompilationException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Compilation of OpenCL program failed with error code " << errorCode << " !";
    return output.str();
}

ProgramReleaseException::ProgramReleaseException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string ProgramReleaseException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Releasing OpenCL program failed with error code " << errorCode << " !";
    return output.str();
}

KernelCreationException::KernelCreationException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string KernelCreationException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Could not create kernel error code " << errorCode << " !";
    return output.str();
}

KernelArgumentException::KernelArgumentException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string KernelArgumentException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Could not add argument to kernel error code " << errorCode << " !";
    return output.str();
}

KernelRunException::KernelRunException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string KernelRunException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Could not run kernel error code " << errorCode << " !";
    return output.str();
}

BufferCreationException::BufferCreationException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string BufferCreationException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Could not create buffer error code " << errorCode << " !";
    return output.str();
}

BufferReadException::BufferReadException(int errorCode) : OpenCLException(createMessage(errorCode)) {}

std::string BufferReadException::createMessage(int errorCode) {
    std::ostringstream output;
    output << "Could not read buffer error code " << errorCode << " !";
    return output.str();
}