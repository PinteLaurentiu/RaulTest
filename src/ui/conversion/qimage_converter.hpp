//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_QIMAGE_CONVERTER_HPP
#define RAULTESTQT_QIMAGE_CONVERTER_HPP


#include <QtGui/QPixmap>
#include <model/image.hpp>
#include <model/image_types.hpp>
#include "qpixel_converter.hpp"

class QImageConverter {
public:
    QImageConverter() = default;
    RGBImage operator()(const QImage&);
    QImage operator()(const RGBImage&);
    QImage operator()(BWImage&);
    QImage operator()(AnyImage&);

private:
    QPixelConverter pixelConverter;
};


#endif //RAULTESTQT_QIMAGE_CONVERTER_HPP
