//
// Created by laurentiu on 03.07.2020.
//

#ifndef RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
#define RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP


#include "../model/image.hpp"

class HistogramEqualization {
public:
    explicit HistogramEqualization(bool adaptive);
    BWImage operator()(BWImage& image);
    static byte getOtsuThreshold(BWImage& image);
    static std::pair<byte, byte> getCannyThresholds(BWImage& image);
private:
    bool adaptive;
    static std::vector<size_t> buildHistogram(BWImage& image);

    BWImage applyLinear(BWImage &image);
    BWImage applyAddaptive(BWImage &image);
};


#endif //RAULTESTQT_HISTOGRAM_EQUALIZATION_HPP
