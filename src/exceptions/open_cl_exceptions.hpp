//
// Created by laurentiu on 31.05.2020.
//

#ifndef RAULTESTQT_OPEN_CL_EXCEPTIONS_HPP
#define RAULTESTQT_OPEN_CL_EXCEPTIONS_HPP

#include "backend_exception.hpp"

class OpenCLException : public BackendException {
public:
    OpenCLException(const std::string& reason);
};

template<typename Stream>
Stream& operator<<(Stream& stream, OpenCLException& exception) {
    return (stream << exception.what());
}

class SourceIOException : public OpenCLException {
public:
    SourceIOException(const std::string& path, const std::string& reason);

private:
    static std::string createMessage(const std::string &path, const std::string &reason);
};

class ProgramCompilationException : public OpenCLException {
public:
    ProgramCompilationException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class ProgramReleaseException : public OpenCLException {
public:
    ProgramReleaseException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class KernelCreationException : public OpenCLException {
public:
    KernelCreationException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class KernelArgumentException : public OpenCLException {
public:
    KernelArgumentException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class KernelRunException : public OpenCLException {
public:
    KernelRunException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class BufferCreationException : public OpenCLException {
public:
    BufferCreationException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

class BufferReadException : public OpenCLException {
public:
    BufferReadException(int errorCode);

private:
    static std::string createMessage(int errorCode);
};

#endif //RAULTESTQT_OPEN_CL_EXCEPTIONS_HPP
