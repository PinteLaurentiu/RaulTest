//
// Created by laurentiu on 05.07.2020.
//

#ifndef RAULTESTQT_BIDIRECTIONAL_MATRIX_TRANSFORMATION_HPP
#define RAULTESTQT_BIDIRECTIONAL_MATRIX_TRANSFORMATION_HPP


#include "../model/matrix_transformation_type.hpp"
#include "../model/image.hpp"

class BidirectionalMatrixTransformation {
public:
    explicit BidirectionalMatrixTransformation(BidirectionalMatrixTransformationType type);
    BWImage operator()(BWImage& image);
    RGBImage operator()(RGBImage& image);
private:
    BidirectionalMatrixTransformationType type;
};


#endif //RAULTESTQT_BIDIRECTIONAL_MATRIX_TRANSFORMATION_HPP
