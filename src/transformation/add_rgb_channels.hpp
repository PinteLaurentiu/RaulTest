//
// Created by laurentiu on 02.07.2020.
//

#ifndef RAULTESTQT_ADD_RGB_CHANNELS_HPP
#define RAULTESTQT_ADD_RGB_CHANNELS_HPP


#include "../model/image.hpp"

struct AddRGBChannels {
    AddRGBChannels() = default;
    RGBImage operator()(BWImage& image);
};


#endif //RAULTESTQT_ADD_RGB_CHANNELS_HPP
