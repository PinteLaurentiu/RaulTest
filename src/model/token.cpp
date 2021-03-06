//
// Created by laurentiu on 25.06.2020.
//

#include <iostream>
#include "token.hpp"
#include <QJsonObject>
#include <QStringList>

TokenStorage& TokenStorage::instance() {
    static TokenStorage instance;
    return instance;
}

Token& TokenStorage::getToken() {
    if (!hasToken()) {
        std::cerr << "No token set" << std::endl;
        std::terminate();
    }
    return token.value();
}

bool TokenStorage::hasToken() {
    return token.has_value();
}

void TokenStorage::saveToken(const Token& newToken) {
    token = newToken;
}

void TokenStorage::clearToken() {
    token = std::optional<Token>();
}

Token::Token(const QJsonDocument& document) {
    auto object = document.object();
    accessToken = object.value("access_token").toString().toStdString();
    userDetails = UserDetails(QJsonDocument(object.value("UserDetailsData").toObject()));
}
