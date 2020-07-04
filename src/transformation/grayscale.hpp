//
// Created by laurentiu on 02.07.2020.
//

#ifndef RAULTESTQT_GRAYSCALE_HPP
#define RAULTESTQT_GRAYSCALE_HPP

#include "../model/image.hpp"

struct Grayscale {
    Grayscale() = default;
    BWImage operator()(RGBImage& image);
};


#endif //RAULTESTQT_GRAYSCALE_HPP
