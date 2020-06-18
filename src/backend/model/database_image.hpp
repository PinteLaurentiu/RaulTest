//
// Created by laurentiu on 18.06.2020.
//

#ifndef RAULTESTQT_DATABASE_IMAGE_HPP
#define RAULTESTQT_DATABASE_IMAGE_HPP

#include <QtCore/QByteArray>
#include "image_info.hpp"
#include "image.hpp"

struct DatabaseImage : ImageInformation {
    DatabaseImage() = default;
    DatabaseImage(long id, long width, long height, bool result, QByteArray imageData, std::string checksum);
    explicit DatabaseImage(const BWImage& image);
    QByteArray imageData;
    std::string checksum;
    explicit operator BWImage();
};


#endif //RAULTESTQT_DATABASE_IMAGE_HPP
