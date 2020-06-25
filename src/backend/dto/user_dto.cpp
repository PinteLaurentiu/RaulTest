//
// Created by laurentiu on 25.06.2020.
//

#include "user_dto.hpp"
#include <QJsonObject>
#include <QJsonArray>

UserDto::UserDto(const QJsonDocument& document) {
    auto object = document.object();
    id = object.value("id").toInt();
    name = object.value("name").toString().toStdString();
    email = object.value("email").toString().toStdString();
    for (auto role : object.value("roles").toArray()) {
        roles.push_back(userRoleFromString(role.toString().toStdString()));
    }
}
