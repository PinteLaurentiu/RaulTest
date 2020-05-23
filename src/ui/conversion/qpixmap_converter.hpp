//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_QPIXMAP_CONVERTER_HPP
#define RAULTESTQT_QPIXMAP_CONVERTER_HPP


#include <QtGui/QPixmap>
#include <image.hpp>
#include "qpixel_converter.hpp"

class QPixmapConverter {
public:
    QPixmapConverter() = default;
    RGBImage operator()(const QPixmap&);
    QPixmap operator()(const RGBImage&);
    RGBImage operator()(const QImage&);
    QImage image(const RGBImage&);
private:
    QPixelConverter pixelConverter;
};


#endif //RAULTESTQT_QPIXMAP_CONVERTER_HPP
