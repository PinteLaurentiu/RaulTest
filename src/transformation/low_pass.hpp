//
// Created by laurentiu on 02.07.2020.
//

#ifndef RAULTESTQT_LOW_PASS_HPP
#define RAULTESTQT_LOW_PASS_HPP


#include "../model/image.hpp"

class LowPass {
public:
    explicit LowPass(int size = 3);
    RGBImage operator()(RGBImage& image);
    BWImage operator()(BWImage& image);

private:
    int size;
};


#endif //RAULTESTQT_LOW_PASS_HPP
