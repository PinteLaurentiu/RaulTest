//
// Created by laurentiu on 09.07.2020.
//

#include "color_inversion.hpp"

BWImage ColorInversion::operator()(BWImage& image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("inversion");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::READ_WRITE,
                              OpenCLBufferMemoryType::DUPLICATE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}

RGBImage ColorInversion::operator()(RGBImage& image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("inversion");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::READ_WRITE,
                              OpenCLBufferMemoryType::DUPLICATE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size() * 3);
    bufferOutput.read(destination.getData());
    return destination;
}
