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
}

