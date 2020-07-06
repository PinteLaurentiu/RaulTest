//
// Created by laurentiu on 05.07.2020.
//

#include "image_history.hpp"

void ImageHistory::push(ImageCache&& image) {
    backStack.push_back(std::move(image));
    frontStack = Stack();
}

ImageCache ImageHistory::popBack(ImageCache&& image) {
    frontStack.push_back(std::move(image));
    auto oldImage = std::move(backStack.back());
    backStack.pop_back();
    return oldImage;
}

ImageCache ImageHistory::popFront(ImageCache&& image) {
    backStack.push_back(std::move(image));
    auto oldImage = std::move(frontStack.back());
    frontStack.pop_back();
    return oldImage;
}

void ImageHistory::clear() {
    frontStack = Stack();
    backStack = Stack();
}

ImageHistory &ImageHistory::instance() {
    static ImageHistory instance;
    return instance;
}

bool ImageHistory::hasBack() {
    return !backStack.empty();
}

bool ImageHistory::hasFront() {
    return !frontStack.empty();
}

ImageCache ImageHistory::returnToOriginal(ImageCache&& image) {
    push(std::move(image));
    return ImageCache(backStack.front().getQImage());
}

