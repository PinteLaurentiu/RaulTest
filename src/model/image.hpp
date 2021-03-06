//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_IMAGE_HPP
#define RAULTESTQT_IMAGE_HPP

#include "types.hpp"
#include "rgb_pixel.hpp"
#include "bw_pixel.hpp"
#include "../opencl/open_cl_kernel.hpp"
#include <memory>

template <typename Pixel>
class Image {
public:
    class PixelRow;
    class ConstPixelRow;
public:
    Image();
    Image(size width, size height);
    Image(const Image&) = delete;
    [[maybe_unused]] Image(Image&&) noexcept;
    Image& operator=(const Image&) = delete ;
    Image& operator=(Image&&) noexcept;
    ~Image() = default;
    std::vector<Pixel>& getData();
    [[nodiscard]] const std::vector<Pixel>& getData() const;
    [[maybe_unused]] void setData(size width, size height, std::vector<Pixel> data);
    [[nodiscard]] size getWidth() const;
    [[nodiscard]] size getHeight() const;
    Pixel& get(size x, size y);
    [[nodiscard]] const Pixel& get(size x, size y) const;
    PixelRow operator[](size x);
    ConstPixelRow operator[](size x) const;
private:
    std::vector<Pixel> data;
    size width;
    size height;
};

template <typename Pixel>
struct Image<Pixel>::PixelRow {
    Image<Pixel>& value;
    size x;
    Pixel& operator[](size y);
    const Pixel& operator[](size y) const;
};

template<typename Pixel>
Pixel& Image<Pixel>::PixelRow::operator[](size y) {
    return value.get(x, y);
}

template<typename Pixel>
const Pixel& Image<Pixel>::PixelRow::operator[](size y) const {
    return value.get(x, y);
}

template <typename Pixel>
struct Image<Pixel>::ConstPixelRow {
    const Image<Pixel>& value;
    size x;
    const Pixel& operator[](size y) const;
};

template<typename Pixel>
const Pixel& Image<Pixel>::ConstPixelRow::operator[](size y) const {
    return value.get(x, y);
}

template<typename Pixel>
Image<Pixel>::Image() : width(0), height(0) {}

template<typename Pixel>
Image<Pixel>::Image(size width, size height) : width(width), height(height), data(width * height, Pixel()) {}

template<typename Pixel>
[[maybe_unused]]
Image<Pixel>::Image(Image&& other) noexcept : width(other.width), height(other.height), data(std::move(other.data)) {}

template<typename Pixel>
Image<Pixel>& Image<Pixel>::operator=(Image&& other) noexcept {
    width = other.width;
    height = other.height;
    data = std::move(other.data);
    return *this;
}

template<typename Pixel>
std::vector<Pixel>& Image<Pixel>::getData() {
    return data;
}

template<typename Pixel>
const std::vector<Pixel>& Image<Pixel>::getData() const {
    return data;
}

template<typename Pixel>
[[maybe_unused]] void Image<Pixel>::setData(size newWidth, size newHeight, std::vector<Pixel> newData) {
    width = newWidth;
    height = newHeight;
    data = std::move(newData);
}

template<typename Pixel>
size Image<Pixel>::getWidth() const {
    return width;
}

template<typename Pixel>
size Image<Pixel>::getHeight() const {
    return height;
}

template<typename Pixel>
Pixel& Image<Pixel>::get(size x, size y) {
    return data[y * width + x];
}

template<typename Pixel>
const Pixel &Image<Pixel>::get(size x, size y) const {
    return data[y * width + x];
}

template<typename Pixel>
typename Image<Pixel>::PixelRow Image<Pixel>::operator[](size x) {
    return Image::PixelRow {*this, x};
}

template<typename Pixel>
typename Image<Pixel>::ConstPixelRow Image<Pixel>::operator[](size x) const {
    return Image::ConstPixelRow{*this, x};
}

using RGBImage = Image<RGBPixel>;
using BWImage = Image<BWPixel>;

#endif //RAULTESTQT_IMAGE_HPP
