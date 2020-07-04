//
// Created by laurentiu on 04.07.2020.
//

#include "matrix_transformation_type.hpp"

std::vector<int> getMatrix(MatrixTransformationType transformation) {
    switch (transformation) {
        case MatrixTransformationType::Laplacian:
            return {-1, -1, -1, -1, 8, -1, -1, -1, -1};
        case MatrixTransformationType::DiagonalLaplacian:
            return {-1, 0, -1, 0, 4, 0, -1, 0, -1};
        case MatrixTransformationType::HorizontalLaplacian:
            return {0, -1, 0, 0, 2, 0, 0, -1, 0};
        case MatrixTransformationType::VerticalLaplacian:
            return {0, 0, 0, -1, 2, -1, 0, 0, 0};
        case MatrixTransformationType::HorizontalPrewitt:
            return {-1, -1, -1, 0, 0, 0, 1, 1, 1};
        case MatrixTransformationType::VerticalPrewitt:
            return {1, 0, -1, 1, 0, -1, 1, 0, -1};
        case MatrixTransformationType::HorizontalSobel:
            return {1, 2, 1, 0, 0, 0, -1, -2, -1};
        case MatrixTransformationType::VerticalSobel:
            return {1, 0, -1, 2, 0, -2, 1, 0, -1};
        case MatrixTransformationType::HorizontalKirsch:
            return {-3, -3, 5, -3, 0, 5, -3, -3, 5};
        case MatrixTransformationType::VerticalKirsch:
            return {5, 5, 5, -3, 0, -3, -3, -3, -3};
        default:
            std::terminate();
    }
}

std::pair<MatrixTransformationType, MatrixTransformationType>
    getDirectionalTypes(BidirectionalMatrixTransformationType type) {
    switch (type) {
        case BidirectionalMatrixTransformationType::Laplacian:
            return {MatrixTransformationType::HorizontalLaplacian, MatrixTransformationType::VerticalLaplacian};
        case BidirectionalMatrixTransformationType::Prewitt:
            return {MatrixTransformationType::HorizontalPrewitt, MatrixTransformationType::VerticalPrewitt};
        case BidirectionalMatrixTransformationType::Sobel:
            return {MatrixTransformationType::HorizontalSobel, MatrixTransformationType::VerticalSobel};
        case BidirectionalMatrixTransformationType::Kirsch:
            return {MatrixTransformationType::HorizontalKirsch, MatrixTransformationType::VerticalKirsch};
        default:
            std::terminate();
    }
}