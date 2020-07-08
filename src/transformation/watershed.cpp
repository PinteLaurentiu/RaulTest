//
// Created by laurentiu on 06.07.2020.
//

#include <queue>
#include <QtCore/QCryptographicHash>
#include <random>
#include <QtGui/QRgb>
#include "watershed.hpp"

RGBImage Watershed::operator()(BWImage &image) {
    BWImage minimums(image.getWidth(), image.getHeight());
    OpenCLKernel kernel("minimum");
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(minimums.getData(), OpenCLBufferMode::WRITE);
    kernel.addNumericArgument(30);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth(), image.getHeight());
    bufferOutput.read(minimums.getData());

    auto labels = createLabels(minimums);
    auto queues = createQueues(image, minimums, labels);
    watershedLabeling(image, minimums, labels, queues);
    std::vector<int> colors = createColors(minimums, labels);
    return pseudocolorateOnLabels(image, labels, colors);
}

std::vector<int> Watershed::createLabels(BWImage& minimums) {
    int newLabel = 0;
    int width = minimums.getWidth();
    int height = minimums.getHeight();
    std::vector<int> labels(minimums.getData().size(), -1);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            auto& pixel = minimums.get(i, j);
            auto& label = labels[index(i,j,width)];
            if (pixel.brightness() == 0 || label != -1) {
                continue;
            }
            label = newLabel++;
            LocationsQueue queue;
            queue.push({i, j});
            while (!queue.empty()) {
                auto location = queue.front();
                queue.pop();
                for (auto neighbour : location.neighbours()) {
                    if (!neighbour.valid(width, height) ||
                        minimums.get(neighbour.x, neighbour.y).brightness() == 0 ||
                        labels[index(neighbour.x, neighbour.y, width)] != -1) {
                        continue;
                    }
                    labels[index(neighbour.x, neighbour.y, width)] = label;
                    queue.push(neighbour);
                }
            }
        }
    }
    return labels;
}

Watershed::LabelsQueues Watershed::createQueues(BWImage &image, BWImage &minimums, std::vector<int>& labels) {
    LabelsQueues queues(256);
    int width = minimums.getWidth();
    int height = minimums.getHeight();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (minimums.get(i, j).brightness() == 0) {
                continue;
            }
            PixelLocation location = {i,j};
            PixelLabel label = {location, labels[index(i, j, width)]};
            queues[image.get(i, j).brightness()].push(label);
        }
    }
    return queues;
}

void Watershed::watershedLabeling(BWImage& image, BWImage& minimums, std::vector<int>& labels, LabelsQueues& queues) {
    bool done = false;
    int width = image.getWidth();
    int height = image.getHeight();
    while (!done) {
        done = true;
        for (auto& queue : queues) {
            if (!queue.empty()) {
                done = false;
                auto label = queue.front();
                queue.pop();
                for (auto neighbour : label.location.neighbours()) {
                    if (!neighbour.valid(width, height) || labels[index(neighbour.x, neighbour.y, width)] != -1)
                        continue;
                    labels[index(neighbour.x, neighbour.y, width)] = label.label;
                    queues[image.get(neighbour.x, neighbour.y).brightness()].push({neighbour, label.label});
                }
                break;
            }
        }
    }
}

std::vector<int> Watershed::createColors(BWImage& minimums, std::vector<int>& labels) {
    auto colorsCount = *std::max_element(labels.begin(), labels.end());
    std::vector<int> colors;
    for (int i = 0; i <= colorsCount; ++i) {
        colors.push_back(i);
    }
    auto hash = QCryptographicHash::hash(
            QByteArray::fromRawData(reinterpret_cast<const char*>(minimums.getData().data()),
                                    minimums.getData().size()),
            QCryptographicHash::Sha1).toStdString();
    auto seed = std::seed_seq(hash.begin(), hash.end());
    auto generator = std::mt19937(seed);
    std::shuffle(colors.begin(), colors.end(), generator);
    return colors;
}

RGBImage Watershed::pseudocolorateOnLabels(BWImage &image, std::vector<int> &labels, std::vector<int>& colors) {
    auto width = image.getWidth();
    auto height = image.getHeight();
    RGBImage destination(width, height);
    if (colors.size() < 2) {
        return destination;
    }
    OpenCLKernel kernel("watershedPseudocoloration");
    OpenCLBuffer bufferInput(image.getData(), OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferLabels(labels, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferColors(colors, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(destination.getData(), OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferLabels);
    kernel.addArgument(bufferColors);
    kernel.addNumericArgument(static_cast<int>(colors.size()));
    kernel.addArgument(bufferOutput);
    kernel(image.getWidth() * image.getHeight());
    bufferOutput.read(destination.getData());
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; ++j) {
//            auto value = colors[labels[index(i, j, width)]] * 1276 / (colors.size() - 1);
//            auto& pixel = destination.get(i, j);
//            if (value < 256) {
//                pixel = RGBPixel(255 - value, 0, 255);
//            } else if (value < 511) {
//                pixel = RGBPixel(0, value - 255, 255);
//            } else if (value < 766) {
//                pixel = RGBPixel(0, 255, 765 - value);
//            } else if (value < 1021) {
//                pixel = RGBPixel(value - 765, 255, 0);
//            } else if (value < 1276) {
//                pixel = RGBPixel(255, 1275 - value, 0);
//            }
//            auto originalPixel = image.get(i,j);
//            pixel = RGBPixel((originalPixel.brightness() * 4 + pixel.red()) / 5,
//                             (originalPixel.brightness() * 4 + pixel.green()) / 5,
//                             (originalPixel.brightness() * 4 + pixel.blue()) / 5);
//        }
//    }
    return destination;
}

int Watershed::index(int i, int j, int width) { return j * width + i; }

Watershed::PixelLocation Watershed::PixelLocation::left() {
    return Watershed::PixelLocation {x - 1, y};
}

Watershed::PixelLocation Watershed::PixelLocation::right() {
    return Watershed::PixelLocation {x + 1, y};
}

Watershed::PixelLocation Watershed::PixelLocation::up() {
    return Watershed::PixelLocation {x, y - 1};
}

Watershed::PixelLocation Watershed::PixelLocation::down() {
    return Watershed::PixelLocation {x, y + 1};
}

bool Watershed::PixelLocation::valid(int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

std::array<Watershed::PixelLocation, 4> Watershed::PixelLocation::neighbours() {
    return std::array<PixelLocation, 4>{left(), right(), up(), down()};
}
