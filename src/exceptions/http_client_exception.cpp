//
// Created by laurentiu on 23.06.2020.
//

#include "http_client_exception.hpp"

HttpClientException::HttpClientException(const std::string& reason) : BackendException(reason) {}

HttpClientConnectionException::HttpClientConnectionException(const std::string& reason) : HttpClientException(reason) {}

DownloadCorruptionException::DownloadCorruptionException() : HttpClientException("Downloaded content was corrupt!") {}
