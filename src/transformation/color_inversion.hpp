//
// Created by laurentiu on 09.07.2020.
//

#ifndef RAULTESTQT_COLOR_INVERSION_HPP
#define RAULTESTQT_COLOR_INVERSION_HPP


#include "../model/image.hpp"

class ColorInversion {
public:
    ColorInversion() = default;
    BWImage operator()(BWImage& image);
    RGBImage operator()(RGBImage& image);
};


#endif //RAULTESTQT_COLOR_INVERSION_HPP
