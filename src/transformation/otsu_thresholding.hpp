//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_OTSU_THRESHOLDING_HPP
#define RAULTESTQT_OTSU_THRESHOLDING_HPP


#include "../model/image.hpp"

class OtsuThresholding {
public:
    OtsuThresholding() = default;
    BWImage operator()(BWImage& image);
};


#endif //RAULTESTQT_OTSU_THRESHOLDING_HPP
