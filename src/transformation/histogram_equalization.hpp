//
// Created by laurentiu on 03.07.2020.
//

#ifndef RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
#define RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP


#include "../model/image.hpp"

class HistogramEqualization {
public:
    HistogramEqualization(bool adaptive);
    BWImage operator()(BWImage& image);

private:
    bool adaptive;
    std::vector<size_t> buildHistogram(BWImage& image);

    BWImage applyLinear(BWImage &image);

    BWImage applyAddaptive(BWImage &image) const;
};


#endif //RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
