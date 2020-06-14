//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_DATABASE_EXCEPTION_HPP
#define RAULTESTQT_DATABASE_EXCEPTION_HPP


#include "backend_exception.hpp"

class DatabaseException : public BackendException {
public:
    explicit DatabaseException(const std::string& reason);
};

class SQLException : public DatabaseException {
public:
    explicit SQLException(const std::string& reason);

private:
    static std::string createMessage(const std::string& reason);
};

class DBNotFoundException : public DatabaseException {
public:
    DBNotFoundException();

private:
    static std::string createMessage();
};

class DBMultipleMatches : public DatabaseException {
public:
    DBMultipleMatches();

private:
    static std::string createMessage();
};


#endif //RAULTESTQT_DATABASE_EXCEPTION_HPP
