//
// Created by laurentiu on 23.06.2020.
//

#ifndef RAULTESTQT_HTTP_CLIENT_EXCEPTION_HPP
#define RAULTESTQT_HTTP_CLIENT_EXCEPTION_HPP

#include "backend_exception.hpp"

class HttpClientException : public BackendException {
public:
    explicit HttpClientException(const std::string& reason);
};

class HttpClientConnectionException : public HttpClientException {
public:
    explicit HttpClientConnectionException(const std::string& reason);
};

class DownloadCorruptionException : public HttpClientException {
public:
    DownloadCorruptionException();
};

#endif //RAULTESTQT_HTTP_CLIENT_EXCEPTION_HPP
