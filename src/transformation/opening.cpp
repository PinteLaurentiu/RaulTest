//
// Created by laurentiu on 05.07.2020.
//

#include "opening.hpp"
#include "dilation.hpp"
#include "erosion.hpp"

Opening::Opening(int size) : size(size) {}

BWImage Opening::operator()(BWImage &image) {
    auto temporary = Erosion(size)(image);
    return Dilation(size)(temporary);
}