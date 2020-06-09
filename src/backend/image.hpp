//
// Created by laurentiu on 23.05.2020.
//

#ifndef RAULTESTQT_IMAGE_HPP
#define RAULTESTQT_IMAGE_HPP

#include "types.hpp"
#include "rgb_pixel.hpp"
#include "bw_pixel.hpp"
#include "open_cl_kernel.hpp"
#include <memory>

template <typename Pixel>
class Image {
public:
    using PixelPtr = std::unique_ptr<Pixel>;
    class PixelRow;
    class ConstPixelRow;
public:
    Image();
    Image(size width, size height);
    Image(const Image&) = delete;
    Image(Image&&) noexcept;
    Image& operator=(const Image&) = delete ;
    Image& operator=(Image&&) noexcept;
    ~Image() = default;
    PixelPtr& data();
    PixelPtr release();
    void setData(size width, size height, PixelPtr data);
    [[nodiscard]] size width() const;
    [[nodiscard]] size height() const;
    Pixel& get(size x, size y);
    const Pixel& get(size x, size y) const;
    PixelRow operator[](size x);
    ConstPixelRow operator[](size x) const;
    template<typename DestPixel>
    explicit operator Image<DestPixel>() const;
private:
    PixelPtr dataPtr;
    size w;
    size h;
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
Image<Pixel>::Image() : w(0), h(0) {}

template<typename Pixel>
Image<Pixel>::Image(size width, size height) : w(width), h(height), dataPtr(new Pixel[width * height]) {}

template<typename Pixel>
Image<Pixel>::Image(Image&& other) noexcept : w(other.w), h(other.h), dataPtr(std::move(other.dataPtr)) {
    other.release();
}

template<typename Pixel>
Image<Pixel>& Image<Pixel>::operator=(Image&& other) noexcept {
    release();
    w = other.w;
    h = other.h;
    dataPtr = std::move(other.dataPtr);
    other.release();
}

template<typename Pixel>
typename Image<Pixel>::PixelPtr& Image<Pixel>::data() {
    return dataPtr;
}

template<typename Pixel>
typename Image<Pixel>::PixelPtr Image<Pixel>::release() {
    w = 0;
    h = 0;
    return std::move(dataPtr);
}

template<typename Pixel>
void Image<Pixel>::setData(size width, size height, Image::PixelPtr data) {
    release();
    w = width;
    h = height;
    dataPtr = std::move(data);
}

template<typename Pixel>
size Image<Pixel>::width() const {
    return w;
}

template<typename Pixel>
size Image<Pixel>::height() const {
    return h;
}

template<typename Pixel>
Pixel &Image<Pixel>::get(size x, size y) {
    return dataPtr.get()[y * w + x];
}

template<typename Pixel>
const Pixel &Image<Pixel>::get(size x, size y) const {
    return dataPtr.get()[y * w + x];
}

template<typename Pixel>
typename Image<Pixel>::PixelRow Image<Pixel>::operator[](size x) {
    return Image::PixelRow {*this, x};
}

template<typename Pixel>
typename Image<Pixel>::ConstPixelRow Image<Pixel>::operator[](size x) const {
    return Image::ConstPixelRow{*this, x};
}

template<typename Pixel>
template<typename DestPixel>
Image<Pixel>::operator Image<DestPixel>() const {
    Image<DestPixel> dest(w, h);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            dest.get(i, j) = static_cast<DestPixel>(get(i, j));
        }
    }
    return dest;
}

template<>
template<>
inline Image<RGBPixel>::operator Image<BWPixel>() const {
    auto size = w * h;
    Image<BWPixel> dest(w, h);
    OpenCLKernel kernel("rgbToBw");
    OpenCLBuffer bufferInput(dataPtr.get(),
                             size,
                             OpenCLBufferMode::READ,
                             OpenCLBufferMemoryType::DUPLICATE);
    OpenCLBuffer bufferOutput(static_cast<decltype(dest.data().get())>(nullptr),
                              size,
                              OpenCLBufferMode::WRITE);
    kernel.addArgument(bufferInput);
    kernel.addArgument(bufferOutput);
    kernel({{size, 0, size}});
    bufferOutput.read(dest.data().get());
}

using RGBImage = Image<RGBPixel>;
using BWImage = Image<BWPixel>;

#endif //RAULTESTQT_IMAGE_HPP
