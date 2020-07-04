//
// Created by laurentiu on 04.07.2020.
//

#ifndef RAULTESTQT_MATRIX_TRANSFORMATION_HPP
#define RAULTESTQT_MATRIX_TRANSFORMATION_HPP


#include "../model/matrix_transformation_type.hpp"
#include "../model/image.hpp"

class MatrixTransformation {
public:
    explicit MatrixTransformation(MatrixTransformationType type);
    BWImage operator()(BWImage& image);
    RGBImage operator()(RGBImage& image);

private:
    MatrixTransformationType type;
};


#endif //RAULTESTQT_MATRIX_TRANSFORMATION_HPP
