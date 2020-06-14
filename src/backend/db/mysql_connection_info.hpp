//
// Created by laurentiu on 12.06.2020.
//

#ifndef RAULTESTQT_MYSQL_CONNECTION_INFO_HPP
#define RAULTESTQT_MYSQL_CONNECTION_INFO_HPP

#include <string_view>

namespace mysql_connection_info {
    constexpr std::string_view host = "localhost";
    constexpr std::string_view user = "root";
    constexpr std::string_view password = "123456";
    constexpr std::string_view database = "images";
    constexpr unsigned int port = 3306;
}

#endif //RAULTESTQT_MYSQL_CONNECTION_INFO_HPP
