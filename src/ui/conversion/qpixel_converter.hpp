//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_QPIXEL_CONVERTER_HPP
#define RAULTESTQT_QPIXEL_CONVERTER_HPP


#include <model/rgb_pixel.hpp>
#include <QtGui/QRgb>

class QPixelConverter {
public:
    QPixelConverter() = default;
    RGBPixel operator()(QRgb);
    QRgb operator()(const RGBPixel&);
};


#endif //RAULTESTQT_QPIXEL_CONVERTER_HPP
