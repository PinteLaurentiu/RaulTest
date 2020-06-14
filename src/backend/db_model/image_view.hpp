//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_VIEW_SSQL_HPP
#define RAULTESTQT_IMAGE_VIEW_SSQL_HPP

#define MYSQLPP_SSQLS_NO_STATICS
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

sql_create_4(ImageViewSsql, 1, 4,
             mysqlpp::sql_bigint, id,
             mysqlpp::sql_int, width,
             mysqlpp::sql_int, height,
             mysqlpp::sql_tinyint, result)

#endif //RAULTESTQT_IMAGE_VIEW_SSQL_HPP
