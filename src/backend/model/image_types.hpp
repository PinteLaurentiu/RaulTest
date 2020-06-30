//
// Created by laurentiu on 30.06.2020.
//

#ifndef RAULTESTQT_IMAGETYPES_HPP
#define RAULTESTQT_IMAGETYPES_HPP

#include <variant>
#include "image.hpp"

enum class ImageType {
    RGB,
    BW
};

ImageType imageTypeFromString(const std::string& role);

std::string imageTypeToString(ImageType userRole);

template<typename>
struct PixelToImageType;

template<>
struct PixelToImageType<RGBPixel> {
    static constexpr const ImageType type = ImageType::RGB;
};

template<>
struct PixelToImageType<BWPixel> {
    static constexpr const ImageType type = ImageType::BW;
};

using AnyImage = std::variant<RGBImage, BWImage>;

#endif //RAULTESTQT_IMAGETYPES_HPP
