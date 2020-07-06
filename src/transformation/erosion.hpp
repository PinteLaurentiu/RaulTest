//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_EROSION_HPP
#define RAULTESTQT_EROSION_HPP


#include "../model/image.hpp"

class Erosion {
public:
    explicit Erosion(int size);
    BWImage operator()(BWImage& image);
private:
    int size;
};


#endif //RAULTESTQT_EROSION_HPP
