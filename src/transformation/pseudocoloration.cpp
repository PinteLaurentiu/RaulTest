//
// Created by laurentiu on 05.07.2020.
//

#include "pseudocoloration.hpp"

RGBImage Pseudocoloration::operator()(BWImage& image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("pseudocoloration");
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}
