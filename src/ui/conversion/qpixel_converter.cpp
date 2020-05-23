//
// Created by laurentiu on 23.05.2020.
//

#include "qpixel_converter.hpp"

RGBPixel QPixelConverter::operator()(QRgb qt) {
    return RGBPixel(qRed(qt), qGreen(qt), qBlue(qt));
}

QRgb QPixelConverter::operator()(const RGBPixel& lib) {
    return qRgb(lib.red(), lib.green(), lib.blue());
}
