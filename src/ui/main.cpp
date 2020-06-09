#include "controller/controller.hpp"
#include <QApplication>
#include <iostream>
#include <CL/cl.h>
#include <open_cl_context.hpp>
#include <open_cl_kernel.hpp>
#include <main_open_cl_program.hpp>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Controller controller;
    controller.show();

    return QApplication::exec();

//    std::string message = "GdkknVnqkc";
//    std::string output(message.size(), ' ');
//    OpenCLKernel kernel("helloworld");
//    OpenCLBuffer bufferInput(message.data(),
//            message.size()+1,
//            OpenCLBufferMode::READ,
//            OpenCLBufferMemoryType::DUPLICATE);
//    OpenCLBuffer bufferOutput(static_cast<char*>(nullptr),
//            output.size() + 1,
//            OpenCLBufferMode::WRITE);
//    kernel.addArgument(bufferInput);
//    kernel.addArgument(bufferOutput);
//    kernel({{message.size(), 0, message.size()}});
//    bufferOutput.read(output.data());
//    std::cout << output << std::endl;

    return EXIT_SUCCESS;
}
