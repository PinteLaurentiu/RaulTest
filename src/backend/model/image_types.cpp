//
// Created by laurentiu on 30.06.2020.
//

#include <src/backend/dto/image_dto.hpp>
#include "image_types.hpp"

ImageType imageTypeFromString(const std::string& type) {
    if (type == "BW")
        return ImageType::BW;
    else
        return ImageType::RGB;
}

std::string imageTypeToString(ImageType userRole) {
    switch (userRole) {
        case ImageType::BW:
            return "BW";
        case ImageType::RGB:
        default:
            return "RGB";
    }
}

