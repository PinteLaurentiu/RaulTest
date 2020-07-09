//
// Created by laurentiu on 09.07.2020.
//

#ifndef RAULTESTQT_GAUSSIAN_BLUR_HPP
#define RAULTESTQT_GAUSSIAN_BLUR_HPP


#include "../model/image.hpp"

class GaussianBlur {
public:
    GaussianBlur() = default;
    BWImage operator()(BWImage& image);
    RGBImage operator()(RGBImage& image);
};


#endif //RAULTESTQT_GAUSSIAN_BLUR_HPP
