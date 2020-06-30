//
// Created by laurentiu on 29.06.2020.
//

#ifndef RAULTESTQT_IMAGE_DTO_HPP
#define RAULTESTQT_IMAGE_DTO_HPP


#include <string>
#include <QtCore/QByteArray>
#include <QtCore/QCryptographicHash>
#include <src/backend/model/image.hpp>
#include <QtCore/QJsonDocument>

struct ImageDto {
    ImageDto() = default;

    template<typename Pixel>
    explicit ImageDto(Image<Pixel>& image);
    explicit ImageDto(const QJsonDocument& document);
    explicit operator QJsonDocument() const;

    template<typename Pixel>
    explicit operator Image<Pixel>() const;

    long id = 0;
    std::string name = "";
    std::string description = "";
    long width = 0;
    long height = 0;
    std::string checksum = "";
    QByteArray imageData;

};

template<typename Pixel>
ImageDto::ImageDto(Image<Pixel>& image): ImageDto() {
    width = image.getWidth();
    height = image.getHeight();
    imageData = QByteArray::fromRawData(reinterpret_cast<const char*>(image.getData().data()),
                                        image.getData().size() * sizeof(Pixel));
    checksum = QCryptographicHash::hash(imageData, QCryptographicHash::Algorithm::Sha1).toHex().toStdString();
}

template<typename Pixel>
ImageDto::operator Image<Pixel>() const {
    Image<Pixel> image(width, height);
    std::copy_n(imageData.constData(),
                image.getData().size() * sizeof(Pixel),
                reinterpret_cast<char*>(image.getData().data()));
    return image;
}


#endif //RAULTESTQT_IMAGE_DTO_HPP
