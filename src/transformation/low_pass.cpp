//
// Created by laurentiu on 02.07.2020.
//

#include "low_pass.hpp"

LowPass::LowPass(int size) : size(size) {}

BWImage LowPass::operator()(BWImage& image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwLowPass");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::WRITE);
    kernel.addNumericArgument(size);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}

RGBImage LowPass::operator()(RGBImage& image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("rgbLowPass");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::WRITE);
    kernel.addNumericArgument(size);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}
