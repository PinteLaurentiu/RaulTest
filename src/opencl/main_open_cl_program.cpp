//
// Created by laurentiu on 31.05.2020.
//

#include <ios>
#include <filesystem>
#include "main_open_cl_program.hpp"
#include "../exceptions/open_cl_exceptions.hpp"
#include "../model/file_data.hpp"

MainOpenCLProgram& MainOpenCLProgram::instance() {
    static MainOpenCLProgram instance;
    return instance;
}

MainOpenCLProgram::MainOpenCLProgram() : OpenCLContextAccess() {
    std::vector<FileData> fileDataSources;
    std::vector<const char *> cStringSources;
    std::vector<size> cStringSourcesLengths;
    for (const auto& file : std::filesystem::directory_iterator(sourcesCommonPath)) {
        try {
            auto fmmString = file.path().string();
            auto& fileData = fileDataSources.emplace_back(fmmString);
            cStringSources.push_back(fileData.getData().c_str());
            cStringSourcesLengths.push_back(fileData.getData().length());
        } catch (std::ios_base::failure& ex) {
            throw SourceIOException(std::string(file.path().string()), ex.what());
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
