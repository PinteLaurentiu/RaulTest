//
// Created by laurentiu on 05.07.2020.
//

#include "binarization.hpp"

Binarization::Binarization(byte value) : value(value) {}

BWImage Binarization::operator()(BWImage &image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwBinarization");
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::WRITE);
    kernel.addNumericArgument(value);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}


