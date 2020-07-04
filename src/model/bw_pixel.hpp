//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_BW_PIXEL_HPP
#define RAULTESTQT_BW_PIXEL_HPP


#include "types.hpp"

class RGBPixel;

class BWPixel {
public:
    explicit BWPixel(byte = 0);
    BWPixel(const BWPixel&) = default;
    BWPixel(BWPixel&&) noexcept = default;
    BWPixel& operator=(const BWPixel&) = default;
    BWPixel& operator=(BWPixel&&) noexcept = default;
    ~BWPixel() = default;
    byte& brightness();
    [[nodiscard]] byte brightness() const;
    explicit operator RGBPixel() const;
private:
    byte b;
};


#endif //RAULTESTQT_BW_PIXEL_HPP
