//
// Created by laurentiu on 23.06.2020.
//

#ifndef RAULTESTQT_HTTP_CLIENT_BUILDER_HPP
#define RAULTESTQT_HTTP_CLIENT_BUILDER_HPP


#include <string>
#include <unordered_map>
#include <regex>
#include <QtCore/QByteArray>
#include <QtCore/QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QHttpMultiPart>
#include "http_request_type.hpp"
#include "json_document_converter.hpp"
#include "../controller/wait_dialog_controller.hpp"

class HttpMultiPartBuilder;

class HttpClientBuilder {
public:
    HttpClientBuilder(const HttpClientBuilder& other) = delete;
    HttpClientBuilder(HttpClientBuilder&& other) noexcept = delete;
    HttpClientBuilder& operator=(const HttpClientBuilder& other) = delete;
    HttpClientBuilder& operator=(HttpClientBuilder&& other) noexcept = delete;
    ~HttpClientBuilder() = default;
    HttpClientBuilder();
    HttpClientBuilder& withType(HttpRequestType type);
    HttpClientBuilder& withUrl(std::string url);
    HttpClientBuilder& withUrlParameter(std::string parameter, std::string value);
    HttpClientBuilder& withQueryParameter(std::string parameter, std::string value);
    HttpClientBuilder& withHeader(std::string header, std::string value);
    HttpClientBuilder& withAuthentication();

    template<typename BodyType>
    HttpClientBuilder& withBody(BodyType&& body);

    HttpMultiPartBuilder withMultipartBody();

    template<typename Argument = QByteArray,
            typename Callable = std::function<void(Argument)>,
            typename = std::enable_if_t<std::is_invocable_v<Callable, Argument> || std::is_void_v<Argument>>>
    HttpClientBuilder& onSuccess(Callable&& callable);

    template<typename Callable,
            typename = std::enable_if_t<std::is_invocable_v<Callable, std::exception_ptr>>>
    HttpClientBuilder& onError(Callable&& callable);

    void execute();

private:
    using BodyVariant = std::variant<QByteArray, std::unique_ptr<QHttpMultiPart>>;

private:
    void validateUrl(const std::string& url);
    void validateParameter(const std::string& parameter, const std::string& value);
    void validateQueryParameter(const std::string& parameter, const std::string& value);
    void validateHeader(const std::string& parameter, const std::string& value);
    void validateRequest();

private:
    HttpRequestType requestType;
    std::string requestUrl;
    std::unordered_map<std::string, std::string> urlParameters;
    std::unordered_map<std::string, std::string> queryParameters;
    std::unordered_map<std::string, std::string> requestHeaders;
    std::optional<BodyVariant> requestBody;
    std::function<void(QByteArray)> successCallback;
    std::function<void(std::exception_ptr)> errorCallback;
    std::optional<std::reference_wrapper<QNetworkAccessManager>> networkAccessManager;

private:
    static const std::regex urlRegex;
    static const std::regex parameterRegex;
    static constexpr std::string_view baseUrl = "http://localhost:8080";
};



template<typename Body>
HttpClientBuilder& HttpClientBuilder::withBody(Body&& body) {
    if constexpr (std::is_same_v<Body, QByteArray> || std::is_same_v<Body, std::unique_ptr<QHttpMultiPart>>) {
        requestBody = std::forward<Body>(body);
    } else if constexpr (std::is_same_v<Body, std::string> || std::is_same_v<Body, const char*>) {
        requestBody = QByteArray::fromStdString(std::forward<Body>(body));
    } else {
        requestBody = JsonDocumentConverterTo<Body>::toJsonDocument(body).toJson(QJsonDocument::Compact);
    }
    return *this;
}


template<typename Argument, typename Callable, typename>
HttpClientBuilder& HttpClientBuilder::onSuccess(Callable&& callable) {
    if constexpr (std::is_void_v<Argument>) {
        successCallback = [callable = std::move(callable)](auto array) {
            callable();
        };
    } else if constexpr (std::is_same_v<Argument, QByteArray>) {
        successCallback = std::move(callable);
    } else if constexpr (std::is_same_v<Argument, std::string>) {
        successCallback = [callable = std::move(callable)](auto array){
            callable(array.toStdString());
        };
    } else {
        successCallback = [callable = std::move(callable)](auto array){
            QJsonParseError error{0, QJsonParseError::NoError};
            auto json = QJsonDocument::fromJson(array, &error);
            if (error.error != QJsonParseError::NoError) {
                throw error;
            }
            callable(JsonDocumentConverterFrom<Argument>::fromJsonDocument(json));
        };
    }
    return *this;
}

template<typename Callable, typename>
HttpClientBuilder& HttpClientBuilder::onError(Callable&& callable) {
    errorCallback = std::move(callable);
    return *this;
}


#endif //RAULTESTQT_HTTP_CLIENT_BUILDER_HPP
