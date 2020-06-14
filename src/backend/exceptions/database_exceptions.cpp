//
// Created by laurentiu on 13.06.2020.
//

#include <sstream>
#include "database_exceptions.hpp"

DatabaseException::DatabaseException(const std::string& reason) : BackendException(reason) {}

SQLException::SQLException(const std::string& reason) : DatabaseException(createMessage(reason)) {}

std::string SQLException::createMessage(const std::string& reason) {
    std::ostringstream output;
    output << "There was an error while running sql " << reason;
    return output.str();
}

DBNotFoundException::DBNotFoundException() : DatabaseException(createMessage()) {}

std::string DBNotFoundException::createMessage() {
    return "Could not find the requested entity in the database";
}

DBMultipleMatches::DBMultipleMatches() : DatabaseException(createMessage()) {}

std::string DBMultipleMatches::createMessage() {
    return "Found more than one entity with this criteria in the database";
}