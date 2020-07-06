//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_IMAGE_HISTORY_HPP
#define RAULTESTQT_IMAGE_HISTORY_HPP


#include "image_cache.hpp"
#include <stack>

class ImageHistory {
public:
    ImageHistory(const ImageHistory& other) = delete;
    ImageHistory(ImageHistory&& other) noexcept = delete;
    ImageHistory& operator=(const ImageHistory& other) = delete;
    ImageHistory& operator=(ImageHistory&& other) noexcept = delete;
    ~ImageHistory() = default;
    static ImageHistory& instance();
    void push(ImageCache&& image);
    ImageCache popBack(ImageCache&& image);
    ImageCache popFront(ImageCache&& image);
    ImageCache returnToOriginal(ImageCache&& image);
    bool hasBack();
    bool hasFront();
    void clear();
private:
    ImageHistory() = default;
    using Stack = std::vector<ImageCache>;
    Stack backStack;
    Stack frontStack;
};

#endif //RAULTESTQT_IMAGE_HISTORY_HPP
