//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_TOKEN_HPP
#define RAULTESTQT_TOKEN_HPP

#include <optional>
#include <QtCore/QJsonDocument>

struct Token {
    Token() = default;
    explicit Token(QJsonDocument document);
    std::string accessToken;
};


class TokenStorage {
public:
    TokenStorage(const TokenStorage&) = delete;
    TokenStorage(TokenStorage&&) noexcept = delete;
    TokenStorage& operator=(const TokenStorage&) = delete;
    TokenStorage& operator=(TokenStorage&&) noexcept = delete;
    ~TokenStorage() = default;
    static TokenStorage& instance();
    Token& getToken();
    bool hasToken();
    void saveToken(const Token& newToken);
    void clearToken();
private:
    TokenStorage() = default;
    std::optional<Token> token;
};



#endif //RAULTESTQT_TOKEN_HPP
