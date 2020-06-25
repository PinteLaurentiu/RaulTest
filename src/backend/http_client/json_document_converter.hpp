//
// Created by laurentiu on 24.06.2020.
//

#ifndef RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP
#define RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

template <typename Type>
struct JsonDocumentConverterFrom {
    static Type fromJsonDocument(const QJsonDocument& document) {
        return static_cast<Type>(document);
    }
};

template<typename Type>
struct JsonDocumentConverterFrom<std::vector<Type>> {
    static std::vector<Type> fromJsonDocument(const QJsonDocument& document) {
        std::vector<Type> vector;
        for (auto item : document.array()) {
            switch (item.type()) {
                case QJsonValue::Null:
                case QJsonValue::Bool:
                case QJsonValue::Double:
                case QJsonValue::String:
                case QJsonValue::Undefined:
                    break;
                case QJsonValue::Array:
                    vector.push_back(JsonDocumentConverterFrom<Type>::fromJsonDocument(QJsonDocument(item.toArray())));
                case QJsonValue::Object:
                    vector.push_back(JsonDocumentConverterFrom<Type>::fromJsonDocument(QJsonDocument(item.toObject())));

            }
        }
        return vector;
    }
};

template <typename Type>
struct JsonDocumentConverterTo {
    static QJsonDocument toJsonDocument(Type t) {
        return static_cast<QJsonDocument>(t);
    }
};

#endif //RAULTESTQT_JSON_DOCUMENT_CONVERTER_HPP
