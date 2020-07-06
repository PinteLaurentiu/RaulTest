//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_OPENING_HPP
#define RAULTESTQT_OPENING_HPP


#include "../model/image.hpp"

class Opening {
public:
    explicit Opening(int size);
    BWImage operator()(BWImage& image);

private:
    int size;
};


#endif //RAULTESTQT_OPENING_HPP
