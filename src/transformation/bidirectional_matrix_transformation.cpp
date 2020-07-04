//
// Created by laurentiu on 05.07.2020.
//

#include "bidirectional_matrix_transformation.hpp"
#include "matrix_transformation.hpp"

BidirectionalMatrixTransformation::BidirectionalMatrixTransformation(BidirectionalMatrixTransformationType type) :
    type(type) {}

BWImage BidirectionalMatrixTransformation::operator()(BWImage& image) {
    auto pair = getDirectionalTypes(type);
    MatrixTransformation firstTrasformation(pair.first);
    MatrixTransformation secondTrasformation(pair.second);
    BWImage first = firstTrasformation(image);
    BWImage second = secondTrasformation(image);
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLBuffer bufferInput1(first.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferInput2(second.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::READ_WRITE);
    OpenCLKernel kernel("bwMatrixJoin");
    kernel.addArgument(bufferInput1);
    kernel.addArgument(bufferInput2);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}

RGBImage BidirectionalMatrixTransformation::operator()(RGBImage& image) {
    auto pair = getDirectionalTypes(type);
    MatrixTransformation firstTrasformation(pair.first);
    MatrixTransformation secondTrasformation(pair.second);
    RGBImage first = firstTrasformation(image);
    RGBImage second = secondTrasformation(image);
    RGBImage destination(image.getWidth(), image.getHeight());
    OpenCLBuffer bufferInput1(first.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferInput2(second.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::READ_WRITE);
    OpenCLKernel kernel("rgbMatrixJoin");
    kernel.addArgument(bufferInput1);
    kernel.addArgument(bufferInput2);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}

