//
// Created by laurentiu on 29.06.2020.
//

#include <QtCore/QJsonObject>
#include "image_dto.hpp"

ImageDto::operator QJsonDocument() const {
    QJsonObject object;
    object.insert("id", QJsonValue(static_cast<int>(id)));
    object.insert("width", QJsonValue(static_cast<int>(width)));
    object.insert("height", QJsonValue(static_cast<int>(height)));
    object.insert("name", QJsonValue(QString::fromStdString(name)));
    object.insert("description", QJsonValue(QString::fromStdString(description)));
    object.insert("checksum", QJsonValue(QString::fromStdString(checksum)));
    object.insert("imageType", QJsonValue(QString::fromStdString(imageTypeToString(imageType))));
    return QJsonDocument(object);
}

ImageDto::ImageDto(const QJsonDocument& document) {
    QJsonObject object = document.object();
    id = object.value("id").toInt();
    width = object.value("width").toInt();
    height = object.value("height").toInt();
    name = object.value("name").toString().toStdString();
    description = object.value("description").toString().toStdString();
    checksum = object.value("checksum").toString().toStdString();
    imageType = imageTypeFromString(object.value("imageType").toString().toStdString());
}

ImageDto::ImageDto(const AnyImage& anyImage) {
    if (std::holds_alternative<BWImage>(anyImage)) {
        construct<BWPixel>(std::get<BWImage>(anyImage));
    } else {
        construct<RGBPixel>(std::get<RGBImage>(anyImage));
    }
}

ImageDto::operator AnyImage() const {
    if (imageType == ImageType::BW)
        return AnyImage(static_cast<BWImage>(*this));
    else
        return AnyImage(static_cast<RGBImage>(*this));
}

ImageDto::ImageDto(ImageDto&& other) noexcept :
    id(other.id),
    width(other.width),
    height(other.height),
    imageType(other.imageType),
    name(std::move(other.name)),
    description(std::move(other.description)),
    checksum(std::move(other.checksum)),
    imageData(std::move(other.imageData)),
    owner(std::move(other.owner))
    {}

ImageDto& ImageDto::operator=(ImageDto&& other) noexcept {
    id = other.id;
    width = other.width;
    height = other.height;
    imageType = other.imageType;
    name = std::move(other.name);
    description = std::move(other.description);
    checksum = std::move(other.checksum);
    imageData = std::move(other.imageData);
    owner = std::move(other.owner);
    return *this;
}
