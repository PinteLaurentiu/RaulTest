//
// Created by laurentiu on 13.06.2020.
//

#include <src/backend/exceptions/database_exceptions.hpp>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <src/backend/model/image.hpp>
#include <QtSql/QSqlError>
#include <QtCore/QCryptographicHash>
#include <src/backend/model/database_image.hpp>
#include "image_repository.hpp"
#include "database_connection.hpp"

ImageRepository& ImageRepository::instance() {
    static ImageRepository instance;
    return instance;
}

void ImageRepository::save(const DatabaseImage& image) {
    auto query = DatabaseConnection::instance().getQuery();
    query.prepare("INSERT INTO image VALUES (NULL, :width, :height, :imageData, :result, :checksum);");
    query.bindValue(":width", QVariant(static_cast<qulonglong>(image.width)));
    query.bindValue(":height", QVariant(static_cast<qulonglong>(image.height)));
    query.bindValue(":imageData", QVariant(image.imageData));
    query.bindValue(":result", QVariant(image.result));
    query.bindValue(":checksum", QVariant(QString::fromStdString(image.checksum)));
    if (!query.exec()) {
        throw SQLException(query.lastError().text().toStdString());
    }
}

DatabaseImage ImageRepository::get(long id) {
    auto query = DatabaseConnection::instance().getQuery();
    query.prepare("SELECT * FROM image WHERE id = :id;");
    query.bindValue(":id", QVariant(static_cast<qulonglong>(id)));
    if (!query.exec()) {
        throw SQLException(query.lastError().text().toStdString());
    }
    if (query.size() < 1) {
        throw DBNotFoundException();
    }
    if (query.size() > 1) {
        throw DBMultipleMatches();
    }
    query.next();
    return DatabaseImage {
        static_cast<long>(query.value("id").toULongLong()),
        static_cast<long>(query.value("width").toULongLong()),
        static_cast<long>(query.value("height").toULongLong()),
        query.value("result").toBool(),
        query.value("imageData").toByteArray(),
        query.value("checksum").toString().toStdString()
    };
}

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
void ImageRepository::remove(long id) {
    auto query = DatabaseConnection::instance().getQuery();
    query.prepare("DELETE FROM image WHERE id = :id;");
    query.bindValue(":id", QVariant(static_cast<qulonglong>(id)));
    if (!query.exec()) {
        throw SQLException(query.lastError().text().toStdString());
    }
}

std::vector<ImageInformation> ImageRepository::getAllInformation() {
    auto query = DatabaseConnection::instance().getQuery();
    query.prepare("SELECT id, width, height, result, checksum FROM image;");
    if (!query.exec()) {
        throw SQLException(query.lastError().text().toStdString());
    }
    std::vector<ImageInformation> views;
    while (query.next())
    {
        views.emplace_back(query.value("id").toLongLong(),
                           query.value("width").toLongLong(),
                           query.value("height").toLongLong(),
                           query.value("result").toBool());
    }
    return views;
}

bool ImageRepository::exists(const DatabaseImage& image) {
    auto query = DatabaseConnection::instance().getQuery();
    query.prepare("SELECT COUNT(*) FROM image WHERE checksum = :checksum;");
    query.bindValue(":checksum", QVariant(QString::fromStdString(image.checksum)));
    if (!query.exec()) {
        throw SQLException(query.lastError().text().toStdString());
    }
    query.next();
    return query.value(0).toULongLong() > 0;
}
