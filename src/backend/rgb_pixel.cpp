//
// Created by laurentiu on 23.05.2020.
//

#include "rgb_pixel.hpp"
#include "bw_pixel.hpp"

RGBPixel::RGBPixel(byte r, byte g, byte b) : r(r), g(g), b(b) {}

RGBPixel::RGBPixel() : RGBPixel(0,0,0) {}

byte& RGBPixel::red() {
    return r;
}

byte& RGBPixel::green() {
    return g;
}

byte& RGBPixel::blue() {
    return b;
}

byte RGBPixel::red() const {
    return r;
}

byte RGBPixel::green() const {
    return g;
}

byte RGBPixel::blue() const {
    return b;
}

RGBPixel::operator BWPixel() const {
    return BWPixel(static_cast<unsigned int>(r + g + b) / 3);
}
