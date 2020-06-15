//
// Created by laurentiu on 13.06.2020.
//

#include <src/backend/db_conversion/image_converter.hpp>
#include <src/backend/db/image_repository.hpp>
#include "image_service.hpp"
#include <src/backend/db_model/image.hpp>

ImageService& ImageService::instance() {
    static ImageService instance;
    return instance;
}

//void ImageService::saveImage(const BWImage& image, bool result) {
//    ImageConverter converter;
//    ImageRepository::instance().save(converter(image, result));
//}
//
//std::vector<ImageView> ImageService::views() {
//    ImageConverter converter;
//    return converter(ImageRepository::instance().getAllView());
//}
//
//BWImage ImageService::get(long id) {
//    ImageConverter converter;
//    return converter(ImageRepository::instance().get(id));
//}
//
//void ImageService::deleteImage(long id) {
//    ImageRepository::instance().remove(id);
//}
