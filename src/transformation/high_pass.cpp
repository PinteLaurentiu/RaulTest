//
// Created by laurentiu on 03.07.2020.
//

#include "high_pass.hpp"
#include "../model/image.hpp"

HighPass::HighPass(int size) : size(size) {}

BWImage HighPass::operator()(BWImage& image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwHighPass");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::READ_WRITE);
    kernel.addNumericArgument(size);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}

RGBImage HighPass::operator()(RGBImage& image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("rgbHighPass");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::READ_WRITE);
    kernel.addNumericArgument(size);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}
