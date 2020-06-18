//
// Created by laurentiu on 18.06.2020.
//

#ifndef RAULTESTQT_SERVICE_EXCEPTIONS_HPP
#define RAULTESTQT_SERVICE_EXCEPTIONS_HPP

#include "backend_exception.hpp"

class DuplicateImageException : public BackendException {
public:
    DuplicateImageException();
};

#endif //RAULTESTQT_SERVICE_EXCEPTIONS_HPP
