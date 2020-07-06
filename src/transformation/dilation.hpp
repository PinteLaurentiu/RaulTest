//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_DILATION_HPP
#define RAULTESTQT_DILATION_HPP


#include "../model/image.hpp"

class Dilation {
public:
    explicit Dilation(int size);
    BWImage operator()(BWImage& image);
private:
    int size;
};


#endif //RAULTESTQT_DILATION_HPP
