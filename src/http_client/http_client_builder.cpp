//
// Created by laurentiu on 23.06.2020.
//

#include <iostream>
#include "http_client_builder.hpp"
#include "http_multi_part_builder.hpp"
#include <QNetworkReply>
#include "../exceptions/http_client_exception.hpp"
#include "../model/token.hpp"

const std::regex HttpClientBuilder::urlRegex("(https?://)?([a-zA-Z0-9-_~]+\\.)*[a-zA-Z0-9-_~]+(:[0-9]+)?"
                                             "((/[a-zA-Z0-9-_~]+)|(/\\{[a-zA-Z0-9-_~]+\\}))*");

const std::regex HttpClientBuilder::parameterRegex("[a-zA-Z0-9-_~]+");

HttpClientBuilder &HttpClientBuilder::withType(HttpRequestType type) {
    if (type == HttpRequestType::UNDEFINED) {
        throw HttpClientException("Invalid request type");
    }
    requestType = type;
    return *this;
}

HttpClientBuilder& HttpClientBuilder::withUrl(std::string url) {
    std::ostringstream urlBuilder;
    urlBuilder << baseUrl << url;
    url = urlBuilder.str();
    validateUrl(url);
    requestUrl = std::move(url);
    urlParameters.clear();
    return *this;
}

void HttpClientBuilder::validateUrl(const std::string& url) {
    if (!std::regex_match(url, urlRegex)) {
        throw HttpClientException("Invalid url format");
    }
}

HttpClientBuilder& HttpClientBuilder::withUrlParameter(std::string parameter, std::string value) {
    validateParameter(parameter, value);
    urlParameters.insert_or_assign(std::move(parameter), std::move(value));
    return *this;
}

void HttpClientBuilder::validateParameter(const std::string& parameter, const std::string& value) {
    validateQueryParameter(parameter, value);
    std::ostringstream parameterBuilder;
    parameterBuilder << "{" << parameter << "}";
    if (requestUrl.find(parameterBuilder.str()) == std::string::npos) {
        throw HttpClientException("Parameter not found in url");
    }
}

HttpClientBuilder &HttpClientBuilder::withQueryParameter(std::string parameter, std::string value) {
    validateQueryParameter(parameter, value);
    queryParameters.insert_or_assign(std::move(parameter), std::move(value));
    return *this;
}

void HttpClientBuilder::validateQueryParameter(const std::string& parameter, const std::string& value) {
    if (requestUrl.empty()) {
        throw HttpClientException("No url given");
    }
    validateHeader(parameter, value);
    if (!std::regex_match(value, parameterRegex)) {
        throw HttpClientException("Invalid parameter value format");
    }
}

HttpClientBuilder& HttpClientBuilder::withAuthentication() {
    if (!TokenStorage::instance().hasToken())
        return *this;
    auto token = TokenStorage::instance().getToken().accessToken;
    std::ostringstream authorizationHeader;
    authorizationHeader << "Bearer " << token;
    return withHeader("Authorization", authorizationHeader.str());
}

HttpClientBuilder &HttpClientBuilder::withHeader(std::string parameter, std::string value) {
    validateHeader(parameter, value);
    requestHeaders.insert_or_assign(std::move(parameter), std::move(value));
    return *this;
}

void HttpClientBuilder::validateHeader(const std::string& parameter, const std::string& value) {
    if (!std::regex_match(parameter, parameterRegex)) {
        throw HttpClientException("Invalid parameter format");
    }
}

void HttpClientBuilder::execute() {
    validateRequest();
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(requestUrl)));
    if (requestType == HttpRequestType::POST &&
       (!requestBody.has_value() || std::holds_alternative<QByteArray>(requestBody.value())) &&
       requestHeaders.find("content-type") == requestHeaders.end()) {
        requestHeaders["content-type"] = "application/json";
    }
    for (auto& header : requestHeaders) {
        request.setRawHeader(QByteArray::fromStdString(header.first), QByteArray::fromStdString(header.second));
    }
    QNetworkReply* reply;
    if (requestType == HttpRequestType::GET) {
        reply = networkAccessManager.value().get().get(request);
    } else if (!requestBody.has_value()) {
        reply = networkAccessManager.value().get().post(request, QByteArray());
    } else if (std::holds_alternative<QByteArray>(requestBody.value())) {
        reply = networkAccessManager.value().get().post(request, std::get<QByteArray>(requestBody.value()));
    } else {
        auto body = std::get<std::unique_ptr<QHttpMultiPart>>(requestBody.value()).release();
        reply = networkAccessManager.value().get().post(request, body);
        body->setParent(reply);
    }
    QObject::connect(reply, &QNetworkReply::finished, [reply,
                                                       successCallback = std::move(successCallback),
                                                       errorCallback = std::move(errorCallback)](){
        try {
            if (reply->error() != QNetworkReply::NoError) {
                throw HttpClientConnectionException(reply->errorString().toStdString());
            }

            successCallback(reply->readAll());
        } catch (...) {
            errorCallback(std::current_exception());
        }
        reply->deleteLater();
    });
}

void HttpClientBuilder::validateRequest() {
    if (requestType == HttpRequestType::UNDEFINED) {
        throw HttpClientException("Invalid request type");
    }
    if (requestUrl.empty()) {
        throw HttpClientException("No url given");
    }
    for (auto& pair : urlParameters) {
        std::ostringstream parameterBuilder;
        parameterBuilder << "{" << pair.first << "}";
        auto parameter = parameterBuilder.str();
        auto location = requestUrl.find(parameter);
        while (location != std::string::npos) {
            requestUrl.replace(location, parameter.size(), pair.second);
            location = requestUrl.find(parameter);
        }
    }
    if (requestUrl.find("{") != std::string::npos || requestUrl.find("}") != std::string::npos) {
        throw HttpClientException("Missing url parameters");
    }
    if (!queryParameters.empty()) {
        std::ostringstream urlBuilder;
        urlBuilder << requestUrl << "?";
        for (auto iterator = queryParameters.begin(); iterator != queryParameters.end();) {
            urlBuilder << iterator->first << "=" << iterator->second;
            ++iterator;
            if (iterator != queryParameters.end()) {
                urlBuilder << "&";
            }
        }
        requestUrl = urlBuilder.str();
    }
    if (!successCallback) {
        throw HttpClientException("No success action given");
    }
    if (!errorCallback) {
        throw HttpClientException("No error action given");
    }
}

HttpClientBuilder::HttpClientBuilder() {
    static QNetworkAccessManager instance;
    networkAccessManager = std::ref(instance);
}

HttpMultiPartBuilder HttpClientBuilder::withMultipartBody() {
    return HttpMultiPartBuilder(*this);
}
