//
// Created by laurentiu on 25.06.2020.
//

#include "authorization_request.hpp"

AuthorizationRequest::AuthorizationRequest(const std::string &username, const std::string &password) {
    std::ostringstream bodyBuilder;
    bodyBuilder << "username=" << QUrl::toPercentEncoding(QString::fromStdString(username)).toStdString() << "&"
                << "password=" << QUrl::toPercentEncoding(QString::fromStdString(password)).toStdString() << "&"
                << "grant_type=password";
    builder.withType(HttpRequestType::POST)
        .withUrl("/oauth/token")
        .withHeader("Authorization", "Basic UVREZXNrdG9wOjJFZlplbExMZjdmU3ZYT3VVZDZv")
        .withHeader("content-type", "application/x-www-form-urlencoded")
        .withBody(QByteArray::fromStdString(bodyBuilder.str()));
}

void AuthorizationRequest::operator()(std::function<void(Token)> onSuccess,
                                      std::function<void(std::exception_ptr)> onError) {
    builder.onSuccess<Token>(onSuccess).onError(onError).execute();
}
