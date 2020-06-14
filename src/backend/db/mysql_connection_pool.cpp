//
// Created by laurentiu on 12.06.2020.
//

#include "mysql_connection_pool.hpp"

MysqlConnectionPool::MysqlConnectionPool() {}

MysqlConnectionPool::~MysqlConnectionPool() {
    clear();
}

mysqlpp::Connection* MysqlConnectionPool::create() {
    mysqlpp::Connection* connection = new mysqlpp::Connection();
    connection->connect(std::string(mysql_connection_info::database).c_str(),
            std::string(mysql_connection_info::host).c_str(),
            std::string(mysql_connection_info::user).c_str(),
            std::string(mysql_connection_info::password).c_str(),
            mysql_connection_info::port);
    connection->set_option(new mysqlpp::ReconnectOption(true));
    return connection;
}

void MysqlConnectionPool::destroy(mysqlpp::Connection* connection) {
    if(connection) {
        delete connection;
    }
}

unsigned int MysqlConnectionPool::max_idle_time() {
    return maxPoolIdleTime;
}

MysqlConnectionPool& MysqlConnectionPool::instance() {
    static MysqlConnectionPool instance;
    return instance;
}
