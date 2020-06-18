//
// Created by laurentiu on 18.06.2020.
//

#include "service_exceptions.hpp"

DuplicateImageException::DuplicateImageException() : BackendException("Image already exists in the database!") {}
