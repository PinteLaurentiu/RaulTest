//
// Created by laurentiu on 05.07.2020.
//

#include "otsu_thresholding.hpp"
#include "histogram_equalization.hpp"
#include "binarization.hpp"


BWImage OtsuThresholding::operator()(BWImage& image) {
    auto threshold = HistogramEqualization::getOtsuThreshold(image);
    return Binarization(threshold)(image);
}
