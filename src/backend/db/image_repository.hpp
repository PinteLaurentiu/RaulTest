//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_REPOSITORY_HPP
#define RAULTESTQT_IMAGE_REPOSITORY_HPP

#include <vector>

#include <src/backend/model/image_info.hpp>
#include <src/backend/model/database_image.hpp>

class ImageRepository {
public:
    ImageRepository(const ImageRepository&) = delete;
    ImageRepository(ImageRepository&&) noexcept = delete;
    ImageRepository& operator=(const ImageRepository&) = delete;
    ImageRepository& operator=(ImageRepository&&) noexcept = delete;
    static ImageRepository& instance();
    void save(const DatabaseImage& image);
    DatabaseImage get(long id);
    std::vector<ImageInformation> getAllInformation();
    bool exists(const DatabaseImage& image);
//    void update(long id, const ImageSsql& image); //TODO
// std::vector<DatabaseImage> getAll(); //TODO
    void remove(long id);
private:
    ImageRepository() = default;

};


#endif //RAULTESTQT_IMAGE_REPOSITORY_HPP
