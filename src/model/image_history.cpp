//
// Created by laurentiu on 05.07.2020.
//

#include "image_history.hpp"

void ImageHistory::push(ImageCache&& image) {
    backStack.push(std::move(image));
    frontStack = Stack();
}

ImageCache ImageHistory::popBack(ImageCache&& image) {
    frontStack.push(std::move(image));
    auto oldImage = std::move(backStack.top());
    backStack.pop();
    return oldImage;
}

ImageCache ImageHistory::popFront(ImageCache&& image) {
    backStack.push(std::move(image));
    auto oldImage = std::move(frontStack.top());
    frontStack.pop();
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

