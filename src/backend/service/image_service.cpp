//
// Created by laurentiu on 13.06.2020.
//

#include <src/backend/db/image_repository.hpp>
#include "image_service.hpp"
#include <src/backend/model/database_image.hpp>
#include <src/backend/exceptions/service_exceptions.hpp>

ImageService& ImageService::instance() {
    static ImageService instance;
    return instance;
}

void ImageService::saveImage(const BWImage& image, bool result) {
    auto databaseImage = static_cast<DatabaseImage>(image);
    databaseImage.result = result;
    if (ImageRepository::instance().exists(databaseImage)) {
        throw DuplicateImageException();
    }
    ImageRepository::instance().save(databaseImage);
}

std::vector<ImageInformation> ImageService::views() {
    return ImageRepository::instance().getAllInformation();
}

BWImage ImageService::get(long id) {
    return static_cast<BWImage>(ImageRepository::instance().get(id));
}

void ImageService::deleteImage(long id) {
    ImageRepository::instance().remove(id);
}
