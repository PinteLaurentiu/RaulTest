//
// Created by laurentiu on 05.07.2020.
//

#include "closing.hpp"
#include "dilation.hpp"
#include "erosion.hpp"

Closing::Closing(int size) : size(size) {}

BWImage Closing::operator()(BWImage &image) {
    auto temporary = Dilation(size)(image);
    return Erosion(size)(temporary);
}
