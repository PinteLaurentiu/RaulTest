//
// Created by laurentiu on 18.06.2020.
//

#include <QtCore/QCryptographicHash>
#include "database_image.hpp"
#include <algorithm>

DatabaseImage::DatabaseImage(long id,
        long width,
        long height,
        bool result,
        QByteArray imageData,
        std::string checksum) :
        ImageInformation(id, width, height, result),
        imageData(std::move(imageData)),
        checksum(std::move(checksum)) {}

DatabaseImage::operator BWImage() {
    BWImage image(width, height);
    std::copy_n(reinterpret_cast<const BWPixel*>(imageData.constData()),
            image.getData().size(),
            image.getData().data());
    return image;
}

DatabaseImage::DatabaseImage(const BWImage& image) {
    id = 0;
    width = image.getWidth();
    height = image.getHeight();
    result = false;
    imageData = QByteArray::fromRawData(reinterpret_cast<const char*>(image.getData().data()), image.getData().size());
    checksum = QCryptographicHash::hash(imageData, QCryptographicHash::Algorithm::Sha1).toHex().toStdString();
}
