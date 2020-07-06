//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_PSEUDOCOLORATION_HPP
#define RAULTESTQT_PSEUDOCOLORATION_HPP


#include "../model/image.hpp"

class Pseudocoloration {
public:
    Pseudocoloration() = default;
    RGBImage operator()(BWImage& image);
};


#endif //RAULTESTQT_PSEUDOCOLORATION_HPP
