//
// Created by laurentiu on 30.06.2020.
//

#include <QtCore/QJsonObject>
#include "owner_dto.hpp"

OwnerDto::OwnerDto(const QJsonDocument& document) {
    QJsonObject object = document.object();
    id = object.value("userId").toInt();
    name = object.value("userName").toString().toStdString();
}
