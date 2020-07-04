//
// Created by laurentiu on 03.07.2020.
//

#include <iostream>
#include "histogram_equalization.hpp"

BWImage HistogramEqualization::operator()(BWImage &image) {
    if (adaptive) {
        return applyAddaptive(image);
    }
    return applyLinear(image);
}

BWImage HistogramEqualization::applyAddaptive(BWImage &image) const {
    auto neighbourhood = (image.getWidth() * image.getHeight()) / 16;
    long long size = 1;
    while (size * size < neighbourhood) {
        ++size;
    }
    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("histogram_equalization_2");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::WRITE);
    kernel.addNumericArgument(static_cast<int>(size));
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(destination.getData());
    return destination;
}

BWImage HistogramEqualization::applyLinear(BWImage &image) {
    auto histogram = buildHistogram(image);
    auto first = std::find_if(histogram.begin(),
                              histogram.end(),
                              [](size_t value) { return value != 0; }) - histogram.begin();
    auto last = histogram.size() - 1 - (std::find_if(histogram.rbegin(),
                                                     histogram.rend(),
                                                     [](size_t value) { return value != 0; }) - histogram.rbegin());
    float multiply = 255.f / static_cast<float>(last - first);
    float offset = first * multiply;

    BWImage destination(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("histogram_equalization");
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(),
                              OpenCLBufferMode::READ_WRITE,
                              OpenCLBufferMemoryType::DUPLICATE);
    kernel.addNumericArgument(multiply);
    kernel.addNumericArgument(offset);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(destination.getData().size());
    bufferOutput.read(destination.getData());
    return destination;
}

std::vector<size_t> HistogramEqualization::buildHistogram(BWImage &image) {
    auto size = image.getWidth() * image.getHeight();

    OpenCLKernel kernel("histogram");
    std::vector<size_t> histogramInit(256 * kernel.getWorkGroupSize(), 0);
    OpenCLBuffer bufferInput(image.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(histogramInit,
                              OpenCLBufferMode::READ_WRITE,
                              OpenCLBufferMemoryType::DUPLICATE);
    kernel.addNumericArgument(size);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    auto event = kernel(kernel.getWorkGroupSize());

    OpenCLKernel joinKernel("histogram_join");
    std::vector<size_t> histogram(256);
    OpenCLBuffer bufferJoinOutput(histogram,
                                  OpenCLBufferMode::WRITE);
    joinKernel.addNumericArgument(kernel.getWorkGroupSize());
    joinKernel.addArgument(bufferOutput);
    joinKernel.addArgument(bufferJoinOutput);
    joinKernel.waitFor(event);
    joinKernel(256);
    bufferJoinOutput.read(histogram);

    return histogram;
}

HistogramEqualization::HistogramEqualization(bool adaptive) : adaptive(adaptive) {}
