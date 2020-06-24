//
// Created by laurentiu on 24.06.2020.
//

#ifndef RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP
#define RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP

#include <QtCore/QJsonDocument>

template <typename Type>
struct JsonDocumentConverterFrom {
    static Type fromJsonDocument(QJsonDocument document) {
        return static_cast<Type>(document);
    }
};

template <typename Type>
struct JsonDocumentConverterTo {
    static QJsonDocument toJsonDocument(Type t) {
        return static_cast<QJsonDocument>(t);
    }
};

#endif //RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP
