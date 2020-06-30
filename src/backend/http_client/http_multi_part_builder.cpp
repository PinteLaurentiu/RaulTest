//
// Created by laurentiu on 28.06.2020.
//

#include "http_multi_part_builder.hpp"
#include <QHttpMultiPart>

HttpMultiPartBuilder::HttpMultiPartBuilder(HttpClientBuilder& parent) : parent(parent),
    body(std::make_unique<QHttpMultiPart>()),
    part(nullptr) {}

HttpClientBuilder& HttpMultiPartBuilder::endMultipart() {
    body->append(*part);
    return parent.withBody(std::move(body));
}

HttpMultiPartBuilder &HttpMultiPartBuilder::withPart() {
    if (part != nullptr)
        body->append(*part);
    part = std::make_unique<QHttpPart>();
    return *this;
}

HttpMultiPartBuilder &HttpMultiPartBuilder::withHeader(const std::string &name, const std::string &value) {
    part->setRawHeader(QByteArray::fromStdString(name), QByteArray::fromStdString(value));
    return *this;
}

HttpMultiPartBuilder &HttpMultiPartBuilder::withBody(const QByteArray& bodyPart) {
    part->setBody(bodyPart);
    return *this;
}

