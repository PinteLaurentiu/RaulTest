//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_INFO_HPP
#define RAULTESTQT_IMAGE_INFO_HPP

#include "types.hpp"
#include <string>

struct ImageInformation {
    ImageInformation() = default;
    ImageInformation(long id, long width, long height, bool result);
    long id;
    long width;
    long height;
    bool result;
};


#endif //RAULTESTQT_IMAGE_INFO_HPP
