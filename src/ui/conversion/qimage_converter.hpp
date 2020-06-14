//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_QIMAGE_CONVERTER_HPP
#define RAULTESTQT_QIMAGE_CONVERTER_HPP


#include <QtGui/QPixmap>
#include <model/image.hpp>
#include "qpixel_converter.hpp"

class QImageConverter {
public:
    QImageConverter() = default;
    RGBImage operator()(const QImage&);
    QImage operator()(const RGBImage&);

private:
    QPixelConverter pixelConverter;
};


#endif //RAULTESTQT_QIMAGE_CONVERTER_HPP
