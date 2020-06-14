//
// Created by laurentiu on 23.05.2020.
//

#include "qimage_converter.hpp"

RGBImage QImageConverter::operator()(const QImage& qt) {
    RGBImage image(qt.width(), qt.height());
    for (int i = 0; i < qt.width(); i++) {
        for (int j = 0; j < qt.height(); j++) {
            image.get(i,j) = pixelConverter(qt.pixel(i,j));
        }
    }
    return image;
}

QImage QImageConverter::operator()(const RGBImage& lib) {
    QImage img(lib.getWidth(), lib.getHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < lib.getWidth(); i++) {
        for (int j = 0; j < lib.getHeight(); j++) {
            img.setPixel(i, j, pixelConverter(lib.get(i, j)));
        }
    }
    return img;
}
