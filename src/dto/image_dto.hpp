//
// Created by laurentiu on 29.06.2020.
//

#ifndef RAULTESTQT_IMAGE_DTO_HPP
#define RAULTESTQT_IMAGE_DTO_HPP


#include <string>
#include <QtCore/QByteArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>
#include "owner_dto.hpp"
#include "../model/image.hpp"
#include "../model/image_types.hpp"

class ImageDto {
public:
    ImageDto() = default;
    ImageDto(const ImageDto& other) = delete;
    ImageDto(ImageDto&& other) noexcept;
    ImageDto& operator=(const ImageDto& other) = delete;
    ImageDto& operator=(ImageDto&& other) noexcept;

    template<typename Pixel>
    explicit ImageDto(const Image<Pixel>& image);
    explicit ImageDto(const QJsonDocument& document);
    explicit ImageDto(const AnyImage& anyImage);

    template<typename Pixel>
    explicit operator Image<Pixel>() const;
    explicit operator QJsonDocument() const;
    explicit operator AnyImage() const;

public:
    long id = 0;
    std::string name = "";
    std::string description = "";
    long width = 0;
    long height = 0;
    std::string checksum = "";
    ImageType imageType = ImageType::RGB;
    QByteArray imageData;
    std::optional<OwnerDto> owner;

private:
    template<typename Pixel>
    void construct(const Image<Pixel>& image);
};

template<typename Pixel>
ImageDto::ImageDto(const Image<Pixel>& image): ImageDto() {
    construct<Pixel>(image);
}

template<typename Pixel>
void ImageDto::construct(const Image<Pixel>& image) {
    width = image.getWidth();
    height = image.getHeight();
    imageData = QByteArray::fromRawData(reinterpret_cast<const char*>(image.getData().data()),
                                        image.getData().size() * sizeof(Pixel));
    checksum = QCryptographicHash::hash(imageData, QCryptographicHash::Algorithm::Sha1).toHex().toStdString();
    imageType = PixelToImageType<Pixel>::type;
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
