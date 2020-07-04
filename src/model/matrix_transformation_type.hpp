//
// Created by laurentiu on 04.07.2020.
//

#ifndef RAULTESTQT_MATRIX_TRANSFORMATION_TYPE_HPP
#define RAULTESTQT_MATRIX_TRANSFORMATION_TYPE_HPP


#include <vector>

enum class MatrixTransformationType {
    Laplacian,
    DiagonalLaplacian,
    HorizontalLaplacian,
    VerticalLaplacian,
    HorizontalPrewitt,
    VerticalPrewitt,
    HorizontalSobel,
    VerticalSobel,
    HorizontalKirsch,
    VerticalKirsch
};

std::vector<int> getMatrix(MatrixTransformationType);

enum class BidirectionalMatrixTransformationType {
    Laplacian,
    Prewitt,
    Sobel,
    Kirsch
};

std::pair<MatrixTransformationType, MatrixTransformationType>
        getDirectionalTypes(BidirectionalMatrixTransformationType type);

#endif //RAULTESTQT_MATRIX_TRANSFORMATION_TYPE_HPP
