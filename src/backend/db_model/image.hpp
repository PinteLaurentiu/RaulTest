//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_DB_HPP
#define RAULTESTQT_IMAGE_DB_HPP

#define MYSQLPP_SSQLS_NO_STATICS
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

sql_create_6(ImageSsql, 1, 6,
             mysqlpp::sql_bigint, id,
             mysqlpp::sql_int, width,
             mysqlpp::sql_int, height,
             std::string, imageData,
             mysqlpp::sql_tinyint, result,
             mysqlpp::sql_varchar, checksum)

#endif //RAULTESTQT_IMAGE_HPP
