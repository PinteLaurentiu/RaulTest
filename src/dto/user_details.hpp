//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_USER_DETAILS_HPP
#define RAULTESTQT_USER_DETAILS_HPP


#include <QtCore/QJsonDocument>
#include "../http_client/json_document_converter.hpp"

enum class UserRole {
    User,
    Admin,
    Generalist,
    Rezident,
    Specialist,
    Primar,
    Unassigned
};

UserRole userRoleFromString(const std::string& role);

std::string userRoleToString(UserRole userRole);

template<>
struct JsonDocumentConverterTo<std::vector<UserRole>> {
    static QJsonDocument toJsonDocument(std::vector<UserRole>& roles);
};

class UserDetails {
public:
    UserDetails() = default;
    explicit UserDetails(const QJsonDocument& document);

public:
    bool isAdmin();

public:
    std::string name;
    std::string email;
    long id = 0;
    std::vector<UserRole> roles;
};


#endif //RAULTESTQT_USER_DETAILS_HPP
