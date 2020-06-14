//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_SERVICE_HPP
#define RAULTESTQT_IMAGE_SERVICE_HPP

#include <src/backend/model/image.hpp>
#include <src/backend/model/image_view.hpp>

class ImageService {
public:
    ImageService(const ImageService&) = delete;
    ImageService(ImageService&&) noexcept = delete;
    ImageService& operator=(const ImageService&) = delete;
    ImageService& operator=(ImageService&&) noexcept = delete;
    static ImageService& instance();
    void saveImage(const BWImage& image, bool result);
    std::vector<ImageView> views();
    BWImage get(long id);
    void deleteImage(long id);
private:
    ImageService() = default;
};

#endif //RAULTESTQT_IMAGE_SERVICE_HPP
