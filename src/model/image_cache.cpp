//
// Created by laurentiu on 01.07.2020.
//

#include "image_cache.hpp"
#include "../transformation/add_rgb_channels.hpp"

ImageCache::ImageCache(QImage qImageValue) : qImage(std::move(qImageValue)) {}

ImageCache::ImageCache(AnyImage qImageValue) : image(std::move(qImageValue)) {}

ImageCache::ImageCache(ImageDto dto) : imageDto(std::move(dto)) {}

QImage& ImageCache::getQImage() {
    if (qImage.isNull())
        loadQImage();
    return qImage;
}

AnyImage& ImageCache::getImage() {
    if (!image)
        loadImage();
    return image.value();
}

ImageDto& ImageCache::getImageDto() {
    if (!imageDto)
        loadImageDto();
    return imageDto.value();
}

void ImageCache::loadQImage() {
    RGBImage rgbImage;
    std::reference_wrapper<RGBImage> imageReference = rgbImage;
    if (std::holds_alternative<RGBImage>(getImage())) {
        imageReference = std::get<RGBImage>(getImage());
    } else {
//        rgbImage = static_cast<RGBImage>(std::get<BWImage>(getImage()));
        rgbImage = AddRGBChannels()(std::get<BWImage>(getImage()));
        imageReference = rgbImage;
    }
    qImage = QImage(imageReference.get().getWidth(), imageReference.get().getHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < imageReference.get().getWidth(); i++) {
        for (int j = 0; j < imageReference.get().getHeight(); j++) {
            auto& pixel = imageReference.get().get(i, j);
            qImage.setPixel(i, j, qRgb(pixel.red(), pixel.green(), pixel.blue()));
        }
    }
}

void ImageCache::loadImage() {
    if (!imageDto) {
        loadImageFromQImage();
    } else {
        loadImageFromImageDto();
    }
}

void ImageCache::loadImageFromQImage() {
    RGBImage rgbImage(qImage.width(), qImage.height());
    for (int i = 0; i < qImage.width(); i++) {
        for (int j = 0; j < qImage.height(); j++) {
            auto pixel = qImage.pixel(i, j);
            rgbImage.get(i,j) = RGBPixel(qRed(pixel), qGreen(pixel), qBlue(pixel));
        }
    }
    image = AnyImage(std::move(rgbImage));
}

void ImageCache::loadImageFromImageDto() {
    image = static_cast<AnyImage>(imageDto.value());
}


void ImageCache::loadImageDto() {
    imageDto = ImageDto(getImage());
}

ImageCache::operator bool() {
    return !qImage.isNull() || image || imageDto;
}

bool ImageCache::hasImageOwner() {
    return hasImageDto() && imageDto->owner.has_value();
}

bool ImageCache::hasImageDto() {
    return imageDto.has_value();
}

bool ImageCache::hasImage() {
    return static_cast<bool>(image);
}



