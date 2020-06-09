//
// Created by laurentiu on 31.05.2020.
//

#include <bits/ios_base.h>
#include <sstream>
#include "main_open_cl_program.hpp"
#include "open_cl_exceptions.hpp"
#include "file_data.hpp"

MainOpenCLProgram& MainOpenCLProgram::instance() {
    static MainOpenCLProgram instance;
    return instance;
}

MainOpenCLProgram::MainOpenCLProgram() : OpenCLContextAccess() {
    std::vector<FileData> fileDataSources;
    std::vector<const char *> cStringSources;
    std::vector<size> cStringSourcesLengths;
    fileDataSources.reserve(sources.size());
    cStringSources.reserve(sources.size());
    cStringSourcesLengths.reserve(sources.size());
    for (auto& source : sources) {
        std::ostringstream pathBuilder;
        pathBuilder << sourcesCommonPath << source;
        try {
            auto fmmString = pathBuilder.str();
            auto& fileData = fileDataSources.emplace_back(fmmString);
            cStringSources.push_back(fileData.getData().c_str());
            cStringSourcesLengths.push_back(fileData.getData().length());
        } catch (std::ios_base::failure& ex) {
            throw SourceIOException(std::string(source), ex.what());
        }
    }
    int errorCode;
    auto programTemporary = clCreateProgramWithSource(context().get(),
            fileDataSources.size(),
            cStringSources.data(),
            cStringSourcesLengths.data(),
            &errorCode);
    if (errorCode != CL_SUCCESS) {
        throw ProgramCompilationException(errorCode);
    }
    auto clDevice = device().value();
    errorCode = clBuildProgram(programTemporary,
            OpenCLContext::expectedDevicesCount,
            &clDevice,
            nullptr,
            nullptr,
            nullptr);
    if (errorCode != CL_SUCCESS) {
        throw ProgramCompilationException(errorCode);
    }
    program = ProgramPtr(programTemporary, &deleteProgram);
}

void MainOpenCLProgram::deleteProgram(cl_program program) {
    if (program == nullptr)
        return;
    auto err = clReleaseProgram(program);
    if (err != CL_SUCCESS)
        throw ProgramReleaseException(err);
}

MainOpenCLProgram::ProgramPtr& MainOpenCLProgramAccess::program() {
    return MainOpenCLProgram::instance().program;
}
