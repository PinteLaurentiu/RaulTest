//
// Created by laurentiu on 02.07.2020.
//

#include "add_rgb_channels.hpp"

RGBImage AddRGBChannels::operator()(BWImage &image) {
    auto size = image.getWidth() * image.getHeight();
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwToRgb");
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
