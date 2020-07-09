//
// Created by laurentiu on 09.07.2020.
//

#ifndef RAULTESTQT_CANNY_HPP
#define RAULTESTQT_CANNY_HPP


#include <queue>
#include "../model/image.hpp"

class Canny {
public:
    Canny() = default;
    BWImage operator()(BWImage& image);
    struct PixelLocation {
        int x = 0;
        int y = 0;
        PixelLocation left();
        PixelLocation right();
        PixelLocation up();
        PixelLocation upLeft();
        PixelLocation upRight();
        PixelLocation down();
        PixelLocation downLeft();
        PixelLocation downRight();
        [[nodiscard]] bool valid(int width, int height) const;
        std::array<PixelLocation, 8> neighbours();
    };
    using LocationsQueue = std::queue<PixelLocation>;

};


#endif //RAULTESTQT_CANNY_HPP
