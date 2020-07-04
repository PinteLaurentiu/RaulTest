//
// Created by laurentiu on 24.06.2020.
//

#include "register_dto.hpp"

RegisterDto::operator QJsonDocument() const {
    QJsonObject object;
    object.insert("name", QJsonValue(QString::fromStdString(name)));
    object.insert("email", QJsonValue(QString::fromStdString(email)));
    object.insert("password", QJsonValue(QString::fromStdString(password)));
    return QJsonDocument(object);
}
