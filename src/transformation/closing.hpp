//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_CLOSING_HPP
#define RAULTESTQT_CLOSING_HPP


#include "../model/image.hpp"

class Closing {
public:
    explicit Closing(int size);
    BWImage operator()(BWImage& image);
private:
    int size;
};


#endif //RAULTESTQT_CLOSING_HPP
