//
// Created by laurentiu on 25.06.2020.
//

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include "user_details.hpp"

UserRole userRoleFromString(const std::string& role)  {
    if (role == "User")
        return UserRole::User;
    else if (role == "Admin")
        return UserRole::Admin;
    else if (role == "Generalist")
        return UserRole::Generalist;
    else if (role == "Rezident")
        return UserRole::Rezident;
    else if (role == "Specialist")
        return UserRole::Specialist;
    else if (role == "Primar")
        return UserRole::Primar;
    else
        return UserRole::Unassigned;
}

std::string userRoleToString(UserRole userRole) {
    switch (userRole) {
        case UserRole::User:
            return "User";
        case UserRole::Admin:
            return "Admin";
        case UserRole::Generalist:
            return "Generalist";
        case UserRole::Rezident:
            return "Rezident";
        case UserRole::Specialist:
            return "Specialist";
        case UserRole::Primar:
            return "Primar";
        case UserRole::Unassigned:
        default:
            return "Unassigned";
    }
}

QJsonDocument JsonDocumentConverterTo<std::vector<UserRole>>::toJsonDocument(std::vector<UserRole>& roles) {
    QJsonArray array;
    for (auto& role : roles)
        array.push_back(QJsonValue(QString::fromStdString(userRoleToString(role))));
    return QJsonDocument(array);
}

UserDetails::UserDetails(const QJsonDocument& document) {
    auto object = document.object();
    id = object.value("id").toInt();
    email = object.value("username").toString().toStdString();
    name = object.value("name").toString().toStdString();
    for (auto role : object.value("roles").toArray()) {
        roles.push_back(userRoleFromString(role.toString().toStdString()));
    }
}

bool UserDetails::isAdmin() {
    return std::find(roles.begin(), roles.end(), UserRole::Admin) != roles.end();
}
