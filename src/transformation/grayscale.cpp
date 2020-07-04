//
// Created by laurentiu on 02.07.2020.
//

#include "grayscale.hpp"

BWImage Grayscale::operator()(RGBImage& image) {
    Image<BWPixel> destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("rgbToBw");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}
