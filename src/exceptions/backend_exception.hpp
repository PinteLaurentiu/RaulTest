//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_BACKEND_EXCEPTION_HPP
#define RAULTESTQT_BACKEND_EXCEPTION_HPP

#include <stdexcept>

class BackendException : public std::runtime_error {
public:
    explicit BackendException(const std::string& message);
};


#endif //RAULTESTQT_BACKEND_EXCEPTION_HPP
