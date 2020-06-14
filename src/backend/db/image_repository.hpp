//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_REPOSITORY_HPP
#define RAULTESTQT_IMAGE_REPOSITORY_HPP

#include <vector>

#include <src/backend/db_model/image.hpp>
#include <src/backend/db_model/image_view.hpp>

class ImageRepository {
public:
    ImageRepository(const ImageRepository&) = delete;
    ImageRepository(ImageRepository&&) noexcept = delete;
    ImageRepository& operator=(const ImageRepository&) = delete;
    ImageRepository& operator=(ImageRepository&&) noexcept = delete;
    static ImageRepository& instance();
    void save(const ImageSsql& image);
    ImageSsql get(long id);
    std::vector<ImageSsql> getAll();
    std::vector<ImageViewSsql> getAllView();
    void update(long id, const ImageSsql& image);
    void remove(long id);
private:
    ImageRepository() = default;

};


#endif //RAULTESTQT_IMAGE_REPOSITORY_HPP
