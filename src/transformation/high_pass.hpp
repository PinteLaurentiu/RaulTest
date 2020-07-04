//
// Created by laurentiu on 03.07.2020.
//

#ifndef RAULTESTQT_HIGH_PASS_HPP
#define RAULTESTQT_HIGH_PASS_HPP


#include "../model/image.hpp"

class HighPass {
public:
    explicit HighPass(int size = 3);
    RGBImage operator()(RGBImage& image);
    BWImage operator()(BWImage& image);

private:
    int size;
};


#endif //RAULTESTQT_HIGH_PASS_HPP
