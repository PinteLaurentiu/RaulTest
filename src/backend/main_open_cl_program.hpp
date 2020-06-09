//
// Created by laurentiu on 31.05.2020.
//

#ifndef RAULTESTQT_MAIN_OPEN_CL_PROGRAM_HPP
#define RAULTESTQT_MAIN_OPEN_CL_PROGRAM_HPP

#include "open_cl_context.hpp"

class MainOpenCLProgram : public OpenCLContextAccess {
public:
    static MainOpenCLProgram& instance();
    friend class MainOpenCLProgramAccess;

private:
    MainOpenCLProgram();

private:
    static void deleteProgram(cl_program program);

private:
    using ProgramPtr = std::unique_ptr<_cl_program, decltype(&deleteProgram)>;

private:
    ProgramPtr program = ProgramPtr(nullptr, &deleteProgram);

private:
    static constexpr std::string_view sourcesCommonPath = "opencl/";
    static constexpr std::array<std::string_view, 1> sources = {"pixel.cl"};
};

class MainOpenCLProgramAccess {
protected:
    MainOpenCLProgram::ProgramPtr& program();
};

#endif //RAULTESTQT_MAIN_OPEN_CL_PROGRAM_HPP
