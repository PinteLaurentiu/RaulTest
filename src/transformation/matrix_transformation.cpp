//
// Created by laurentiu on 04.07.2020.
//

#include "matrix_transformation.hpp"

MatrixTransformation::MatrixTransformation(MatrixTransformationType type) : type(type) {}

BWImage MatrixTransformation::operator()(BWImage& image) {
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("bwMatrix");
    auto matrix = getMatrix(type);
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

RGBImage MatrixTransformation::operator()(RGBImage& image) {
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("rgbMatrix");
    auto matrix = getMatrix(type);
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
