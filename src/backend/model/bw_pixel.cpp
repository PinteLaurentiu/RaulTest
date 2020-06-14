//
// Created by laurentiu on 23.05.2020.
//

#include "bw_pixel.hpp"
#include "rgb_pixel.hpp"

BWPixel::BWPixel(byte b) : b(b) {}

byte& BWPixel::brightness() {
    return b;
}

byte BWPixel::brightness() const {
    return b;
}

BWPixel::operator RGBPixel() const {
    return RGBPixel(b, b, b);
}
