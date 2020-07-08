//
// Created by laurentiu on 01.07.2020.
//

#ifndef RAULTESTQT_IMAGE_CACHE_HPP
#define RAULTESTQT_IMAGE_CACHE_HPP

#include <QtGui/QImage>
#include "image_types.hpp"
#include "../dto/image_dto.hpp"

class ImageCache {
public:
    explicit ImageCache(QImage qImageValue);
    explicit ImageCache(AnyImage imageValue);
    explicit ImageCache(ImageDto dto);
    QImage& getQImage();
    AnyImage& getImage();
    ImageDto& getImageDto();
    explicit operator bool();
    bool hasImage();

private:
    QImage qImage;
    std::optional<AnyImage> image;
    std::optional<ImageDto> imageDto;
    void loadQImage();
    void loadImage();
    bool hasImageDto();
    void loadImageDto();
    void loadImageFromQImage();
    void loadImageFromImageDto();
};


#endif //RAULTESTQT_IMAGE_CACHE_HPP
