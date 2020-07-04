//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_RGB_PIXEL_HPP
#define RAULTESTQT_RGB_PIXEL_HPP

#include "types.hpp"

class BWPixel;

class RGBPixel {
public:
    RGBPixel();
    RGBPixel(byte, byte, byte);
    RGBPixel(const RGBPixel&) = default;
    RGBPixel(RGBPixel&&) noexcept = default;
    RGBPixel& operator=(const RGBPixel&) = default;
    RGBPixel& operator=(RGBPixel&&) noexcept = default;
    ~RGBPixel() = default;
    byte& red();
    byte& green();
    byte& blue();
    [[nodiscard]] byte red() const;
    [[nodiscard]] byte green() const;
    [[nodiscard]] byte blue() const;
    explicit operator BWPixel() const;
private:
    byte r, g, b;
};

#endif //RAULTESTQT_RGB_PIXEL_HPP
