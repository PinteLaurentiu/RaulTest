//
// Created by laurentiu on 12.06.2020.
//

#ifndef RAULTESTQT_MYSQL_CONNECTION_POOL_HPP
#define RAULTESTQT_MYSQL_CONNECTION_POOL_HPP

#include <memory>
#include "mysql_connection_info.hpp"
#include <QSqlDatabase>

class MysqlConnectionPool  {
public:
    MysqlConnectionPool(const MysqlConnectionPool&) = delete;
    MysqlConnectionPool(MysqlConnectionPool&&) noexcept = delete;
    MysqlConnectionPool& operator=(const MysqlConnectionPool&) = delete;
    MysqlConnectionPool& operator=(MysqlConnectionPool&&) noexcept = delete;
    static MysqlConnectionPool& instance();
    ~MysqlConnectionPool();
    QSqlDatabase getConnection();

private:
    MysqlConnectionPool();
};




#endif //RAULTESTQT_MYSQL_CONNECTION_POOL_HPP
