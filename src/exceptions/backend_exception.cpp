//
// Created by laurentiu on 13.06.2020.
//

#include "backend_exception.hpp"

BackendException::BackendException(const std::string& message) : std::runtime_error(message) {}
