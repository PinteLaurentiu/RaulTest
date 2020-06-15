//
// Created by laurentiu on 13.06.2020.
//

#include <src/backend/exceptions/database_exceptions.hpp>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <src/backend/model/image.hpp>
#include "image_repository.hpp"
#include "mysql_connection_pool.hpp"

ImageRepository& ImageRepository::instance() {
    static ImageRepository instance;
    return instance;
}

//void ImageRepository::save(const BWImage& image) {
//    QSqlQuery query("INSERT INTO image VALUES (NULL, :width, :height, :imageData, :result, :checksum)");
//    query.bindValue("width", QVariant(static_cast<qulonglong>(image.getWidth())));
//    query.bindValue("height", QVariant(static_cast<qulonglong>(image.getHeight())));
//    query.bindValue("imageData", QVariant())
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query.insert(image);
//        query.execute();
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}
//
//ImageSsql ImageRepository::get(long id) {
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query << "SELECT * FROM image where id = " << id << ";";
//        std::vector<ImageSsql> result;
//        query.storein(result);
//        if (result.empty()) {
//            throw DBNotFoundException();
//        } else if (result.size() > 1) {
//            throw DBMultipleMatches();
//        }
//        return result[0];
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}

//std::vector<ImageSsql> ImageRepository::getAll() {
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query << "SELECT * FROM image;";
//        std::vector<ImageSsql> result;
//        query.storein(result);
//        return result;
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}
//
//void ImageRepository::update(long id, const ImageSsql& image) {
//    auto old = get(id);
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query.update(old, image);
//        query.execute();
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}
//
//void ImageRepository::remove(long id) {
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query << "DELETE FROM image WHERE id = " << id << ";";
//        query.execute();
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}
//
//std::vector<ImageViewSsql> ImageRepository::getAllView() {
//    mysqlpp::ScopedConnection connection(MysqlConnectionPool::instance());
//    try {
//        auto query = connection->query();
//        query << "SELECT id, width, height, result FROM image;";
//        std::vector<ImageViewSsql> result;
//        query.storein(result);
//        return result;
//    } catch (mysqlpp::Exception& ex) {
//        throw SQLException(ex.what());
//    }
//}
