//
// Created by laurentiu on 12.06.2020.
//

#ifndef RAULTESTQT_MYSQL_CONNECTION_POOL_HPP
#define RAULTESTQT_MYSQL_CONNECTION_POOL_HPP

#include <mysql++/mysql++.h>
#include "mysql_connection_info.hpp"

class MysqlConnectionPool : public mysqlpp::ConnectionPool {
public:
    MysqlConnectionPool(const MysqlConnectionPool&) = delete;
    MysqlConnectionPool(MysqlConnectionPool&&) noexcept = delete;
    MysqlConnectionPool& operator=(const MysqlConnectionPool&) = delete;
    MysqlConnectionPool& operator=(MysqlConnectionPool&&) noexcept = delete;
    static MysqlConnectionPool& instance();
    ~MysqlConnectionPool() override;
    mysqlpp::Connection* create() override;
    void destroy(mysqlpp::Connection* connection) override;
    unsigned int max_idle_time() override;

private:
    MysqlConnectionPool();
    constexpr static unsigned int maxPoolIdleTime = 180;

};




#endif //RAULTESTQT_MYSQL_CONNECTION_POOL_HPP
