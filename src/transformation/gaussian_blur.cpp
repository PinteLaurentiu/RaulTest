//
// Created by laurentiu on 09.07.2020.
//

#include "gaussian_blur.hpp"

BWImage GaussianBlur::operator()(BWImage &image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwGaussian");
    std::vector<int> matrix = {1,  4,  7,  4, 1,
                               4, 16, 26, 16, 4,
                               7, 26, 41, 26, 7,
                               4, 16, 26, 16, 4,
                               1,  4,  7,  4, 1};
    OpenCLBuffer bufferMatrix(matrix, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferMatrix);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}

RGBImage GaussianBlur::operator()(RGBImage &image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("rgbGaussian");
    std::vector<int> matrix = {1,  4,  7,  4, 1,
                               4, 16, 26, 16, 4,
                               7, 26, 41, 26, 7,
                               4, 16, 26, 16, 4,
                               1,  4,  7,  4, 1};
    OpenCLBuffer bufferMatrix(matrix, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferMatrix);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}

