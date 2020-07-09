//
// Created by laurentiu on 09.07.2020.
//

#include "canny.hpp"
#include "gaussian_blur.hpp"
#include "../model/matrix_transformation_type.hpp"
#include "histogram_equalization.hpp"

BWImage Canny::operator()(BWImage &image) {
    auto blurred = GaussianBlur()(image);
    auto vmatrix = getMatrix(MatrixTransformationType::VerticalSobel);
    OpenCLBuffer bufferVMatrix(vmatrix, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    auto hmatrix = getMatrix(MatrixTransformationType::HorizontalSobel);
    OpenCLBuffer bufferHMatrix(hmatrix, OpenCLBufferMode::READ, OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferInput(image.getData(),
                            OpenCLBufferMode::READ,
                            OpenCLBufferMemoryType::DUPLICATE);
    std::vector<float> cannyOut(blurred.getData().size() * 2);
    OpenCLBuffer bufferTmp(cannyOut,
                           OpenCLBufferMode::READ_WRITE,
                           OpenCLBufferMemoryType::DUPLICATE);
    OpenCLKernel kernel("canny");
    kernel.addArgument(bufferVMatrix);
    kernel.addArgument(bufferHMatrix);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferTmp);
    auto event = kernel(image.getData().size());

    BWImage canny2Out(image.getWidth(), image.getHeight());
    OpenCLBuffer bufferTmp2(canny2Out.getData(),
                            OpenCLBufferMode::WRITE);
    OpenCLKernel kernel2("canny2");
    kernel2.addArgument(bufferTmp);
    kernel2.addArgument(bufferTmp2);
    kernel2.waitFor(event);
    kernel2(image.getData().size());
    bufferTmp2.read(canny2Out.getData());

    OpenCLBuffer bufferInput2(canny2Out.getData(),
                              OpenCLBufferMode::READ,
                              OpenCLBufferMemoryType::DUPLICATE);
    BWImage canny3Out(image.getWidth(), image.getHeight());
    OpenCLBuffer bufferTmp3(canny3Out.getData(),
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    auto thresholds = HistogramEqualization::getCannyThresholds(canny2Out);
    OpenCLKernel kernel3("canny3");
    kernel3.addNumericArgument(thresholds.first);
    kernel3.addNumericArgument(thresholds.second);
    kernel3.addArgument(bufferInput2);
    kernel3.addArgument(bufferTmp3);
    kernel3(image.getData().size());
    bufferTmp3.read(canny3Out.getData());

    for (auto i = 0; i < canny3Out.getWidth(); ++i) {
        for (auto j = 0; j < canny3Out.getHeight(); ++j) {
            auto& value = canny3Out.get(i,j).brightness();
            if (value == 0 || value == 255)
                continue;
            std::vector<PixelLocation> locations;
            LocationsQueue queue;
            queue.push({i, j});
            locations.push_back(queue.front());
            byte found = 0;
            while (!queue.empty()) {
                auto location = queue.front();
                queue.pop();
                for (auto& neighbour : location.neighbours()) {
                    if (!neighbour.valid(canny3Out.getWidth(), canny3Out.getHeight()))
                        continue;
                    auto iter = std::find_if(locations.begin(), locations.end(), [&neighbour](auto location) {
                            return neighbour.x == location.x && neighbour.y == location.y;
                    });
                    if (iter != locations.end())
                        continue;
                    auto neighbourValue = canny3Out.get(neighbour.x, neighbour.y).brightness();
                    if (neighbourValue == 0)
                        continue;
                    if (neighbourValue == 255 || neighbourValue == 128) {
                        found = 128;
                        continue;
                    }
                    queue.push(neighbour);
                    locations.push_back(neighbour);
                }
            }
            for (auto& location : locations) {
                canny3Out.get(location.x, location.y).brightness() = found;
            }
        }
    }

    return canny3Out;
}

Canny::PixelLocation Canny::PixelLocation::left() {
    return PixelLocation {x - 1, y};
}

Canny::PixelLocation Canny::PixelLocation::right() {
    return PixelLocation {x + 1, y};
}

Canny::PixelLocation Canny::PixelLocation::up() {
    return PixelLocation {x, y - 1};
}

Canny::PixelLocation Canny::PixelLocation::upLeft() {
    return PixelLocation {x - 1, y - 1};
}

Canny::PixelLocation Canny::PixelLocation::upRight() {
    return PixelLocation {x + 1, y - 1};
}

Canny::PixelLocation Canny::PixelLocation::down() {
    return PixelLocation {x, y + 1};
}

Canny::PixelLocation Canny::PixelLocation::downLeft() {
    return PixelLocation {x - 1, y + 1};
}

Canny::PixelLocation Canny::PixelLocation::downRight() {
    return PixelLocation {x + 1, y + 1};
}

bool Canny::PixelLocation::valid(int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

std::array<Canny::PixelLocation, 8> Canny::PixelLocation::neighbours() {
    return std::array<PixelLocation, 8>{left(), right(), up(), upLeft(), upRight(), down(), downLeft(), downRight()};
}

