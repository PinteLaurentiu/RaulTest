//
// Created by laurentiu on 12.06.2020.
//


#include <iostream>
#include "mysql_connection_pool.hpp"
#include <QSqlError>

MysqlConnectionPool::MysqlConnectionPool() {
    auto database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(QString::fromStdString(std::string(mysql_connection_info::host)));
    database.setDatabaseName(QString::fromStdString(std::string(mysql_connection_info::database)));
    database.setUserName(QString::fromStdString(std::string(mysql_connection_info::user)));
    database.setPassword(QString::fromStdString(std::string(mysql_connection_info::password)));
    database.setPort(mysql_connection_info::port);
    std::cout << database.lastError().text().toStdString() << std::endl;
    if (!database.open()) {
        std::cerr << "Could not connect to the database" << std::endl;
        std::terminate();
    }
}

MysqlConnectionPool::~MysqlConnectionPool() {
    QSqlDatabase::database().close();
}

QSqlDatabase MysqlConnectionPool::getConnection() {
    return QSqlDatabase::database();
}

MysqlConnectionPool& MysqlConnectionPool::instance() {
    static MysqlConnectionPool instance;
    return instance;
}
