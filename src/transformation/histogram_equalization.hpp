//
// Created by laurentiu on 03.07.2020.
//

#ifndef RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
#define RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP


#include "../model/image.hpp"

class HistogramEqualization {
public:
    HistogramEqualization() = default;
//    BWImage operator()(BWImage& image);
    BWImage operator()(BWImage& image);
    std::vector<size_t> buildHistogram(BWImage& image);
};


#endif //RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
