//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_BINARIZATION_HPP
#define RAULTESTQT_BINARIZATION_HPP


#include "../model/types.hpp"
#include "../model/image.hpp"

class Binarization {
public:
    explicit Binarization(byte value);
    BWImage operator()(BWImage& image);
private:
    byte value;
};


#endif //RAULTESTQT_BINARIZATION_HPP
