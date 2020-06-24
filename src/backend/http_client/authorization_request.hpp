//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_AUTHORIZATION_REQUEST_HPP
#define RAULTESTQT_AUTHORIZATION_REQUEST_HPP

#include <string>
#include <src/backend/model/token.hpp>
#include <functional>
#include "http_client_builder.hpp"

class AuthorizationRequest {
public:
    AuthorizationRequest(const std::string& username, const std::string& password);
    void operator()(std::function<void(Token)> onSuccess, std::function<void(std::exception_ptr)> onError);

private:
    HttpClientBuilder builder;
};


#endif //RAULTESTQT_AUTHORIZATION_REQUEST_HPP
