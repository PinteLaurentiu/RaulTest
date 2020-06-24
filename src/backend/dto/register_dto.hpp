//
// Created by laurentiu on 24.06.2020.
//

#ifndef RAULTESTQT_REGISTER_DTO_HPP
#define RAULTESTQT_REGISTER_DTO_HPP

#include <string>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

struct RegisterDto {
    RegisterDto() = default;

    std::string name = "";
    std::string email = "";
    std::string password = "";

    operator QJsonDocument() {
        QJsonObject object;
        object.insert("name", QJsonValue(QString::fromStdString(name)));
        object.insert("email", QJsonValue(QString::fromStdString(email)));
        object.insert("password", QJsonValue(QString::fromStdString(password)));
        return QJsonDocument(object);
    }
};


#endif //RAULTESTQT_REGISTER_DTO_HPP
