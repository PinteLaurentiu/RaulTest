//
// Created by laurentiu on 17.06.2020.
//

#include "database_connection.hpp"
#include "mysql_connection_info.hpp"
#include <QSqlError>
#include <QSqlQuery>
#include <src/backend/exceptions/database_exceptions.hpp>
#include <iostream>

DatabaseConnection:: DatabaseConnection() {
    connection = QSqlDatabase::addDatabase("QMYSQL");
    connection.setHostName(QString::fromStdString(std::string(mysql_connection_info::host)));
    connection.setDatabaseName(QString::fromStdString(std::string(mysql_connection_info::database)));
    connection.setUserName(QString::fromStdString(std::string(mysql_connection_info::user)));
    connection.setPassword(QString::fromStdString(std::string(mysql_connection_info::password)));
    connection.setPort(mysql_connection_info::port);

    if (!connection.open()) {
        std::cerr << connection.lastError().text().toStdString() << std::endl;
        std::terminate();
    }
}

QSqlQuery DatabaseConnection::getQuery() {
    return QSqlQuery(connection);
}

DatabaseConnection& DatabaseConnection::instance() {
    static DatabaseConnection instance;
    return instance;
}
