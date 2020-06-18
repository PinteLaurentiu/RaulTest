//
// Created by laurentiu on 17.06.2020.
//

#ifndef RAULTESTQT_DATABASECONNECTION_HPP
#define RAULTESTQT_DATABASECONNECTION_HPP

#include <QtSql/QSqlDatabase>

class DatabaseConnection {
public:
    DatabaseConnection(const DatabaseConnection& other) = delete;
    DatabaseConnection(DatabaseConnection&& other) noexcept = delete;
    DatabaseConnection& operator= (const DatabaseConnection& other) = delete;
    DatabaseConnection& operator= (DatabaseConnection&& other) noexcept = delete;
    ~DatabaseConnection() = default;
    QSqlQuery getQuery();
    static DatabaseConnection& instance();
private:
    DatabaseConnection();
    QSqlDatabase connection;
};


#endif //RAULTESTQT_DATABASECONNECTION_HPP
