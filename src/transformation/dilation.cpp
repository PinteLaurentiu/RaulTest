//
// Created by laurentiu on 05.07.2020.
//

#include "dilation.hpp"

Dilation::Dilation(int size) : size(size) {}

BWImage Dilation::operator()(BWImage& image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwDilation");
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
