//
// Created by laurentiu on 30.06.2020.
//

#ifndef RAULTESTQT_OWNER_DTO_HPP
#define RAULTESTQT_OWNER_DTO_HPP


#include <string>
#include <QtCore/QJsonDocument>

struct OwnerDto {
    OwnerDto() = default;
    explicit OwnerDto(const QJsonDocument& document);

    long id = 0;
    std::string name = "";
};


#endif //RAULTESTQT_OWNER_DTO_HPP
