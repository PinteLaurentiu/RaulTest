//
// Created by laurentiu on 28.06.2020.
//

#ifndef RAULTESTQT_HTTP_MULTI_PART_BUILDER_HPP
#define RAULTESTQT_HTTP_MULTI_PART_BUILDER_HPP


#include "http_client_builder.hpp"
#include <QHttpPart>


class HttpMultiPartBuilder {
public:
    HttpMultiPartBuilder(const HttpMultiPartBuilder& other) = delete;
    HttpMultiPartBuilder(HttpMultiPartBuilder&& other) noexcept = delete;
    HttpMultiPartBuilder& operator=(const HttpMultiPartBuilder& other) = delete;
    HttpMultiPartBuilder& operator=(HttpMultiPartBuilder&& other) noexcept = delete;
    ~HttpMultiPartBuilder() = default;
    friend class HttpClientBuilder;
    HttpClientBuilder& endMultipart();
    HttpMultiPartBuilder& withPart();

    template<typename Body>
    HttpMultiPartBuilder& withPart(Body bodyPart, const std::string& name);

    HttpMultiPartBuilder& withHeader(const std::string& name, const std::string& value);
    HttpMultiPartBuilder& withBody(const QByteArray& bodyPart);

private:
    explicit HttpMultiPartBuilder(HttpClientBuilder& parent);
    HttpClientBuilder& parent;
    std::unique_ptr<QHttpMultiPart> body;
    std::unique_ptr<QHttpPart> part;
};



template<typename Body>
HttpMultiPartBuilder &HttpMultiPartBuilder::withPart(Body bodyPart, const std::string& name) {
    std::ostringstream contentDispositionName;
    contentDispositionName << "form-data; name=\"" << name << "\"";
    if constexpr (std::is_same_v<Body, QByteArray>) {
        return this->withPart()
                    .withHeader("Content-Type","application/octet-stream")
                    .withHeader("Content-Disposition", contentDispositionName.str())
                    .withBody(std::move(bodyPart));
    } else if constexpr (std::is_same_v<Body, std::string> || std::is_same_v<Body, const char*>) {
        return this->withPart()
                    .withHeader("Content-Type","application/octet-stream")
                    .withHeader("Content-Disposition", contentDispositionName.str())
                    .withBody(QByteArray::fromStdString(std::move(bodyPart)));
    } else {
        auto json = JsonDocumentConverterTo<Body>::toJsonDocument(std::move(bodyPart)).toJson(QJsonDocument::Compact);
        return this->withPart()
                    .withHeader("Content-Type","application/json")
                    .withHeader("Content-Disposition", contentDispositionName.str())
                    .withBody(std::move(json));
    }
}


#endif //RAULTESTQT_HTTP_MULTI_PART_BUILDER_HPP
