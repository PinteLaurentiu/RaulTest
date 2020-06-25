//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_USER_DTO_HPP
#define RAULTESTQT_USER_DTO_HPP


#include "user_details.hpp"

struct UserDto {
    UserDto() = default;
    explicit UserDto(const QJsonDocument& document);
    long id = 0;
    std::string name;
    std::string email;
    std::vector<UserRole> roles;
};


#endif //RAULTESTQT_USER_DTO_HPP
