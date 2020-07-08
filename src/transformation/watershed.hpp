//
// Created by laurentiu on 06.07.2020.
//

#ifndef RAULTESTQT_WATERSHED_HPP
#define RAULTESTQT_WATERSHED_HPP

#include <queue>
#include "../model/image.hpp"

class Watershed {
public:
    Watershed() = default;
    RGBImage operator()(BWImage& image);

private:
    struct PixelLocation {
        int x = 0;
        int y = 0;
        PixelLocation left();
        PixelLocation right();
        PixelLocation up();
        PixelLocation down();
        [[nodiscard]] bool valid(int width, int height) const;
        std::array<PixelLocation, 4> neighbours();
    };
    struct PixelLabel {
        PixelLocation location {0, 0};
        int label = 0;
    };
    using LocationsQueue = std::queue<PixelLocation>;
    using LabelsQueues = std::vector<std::queue<PixelLabel>>;

    [[nodiscard]] static int index(int x, int y, int width);
    static std::vector<int> createLabels(BWImage& minimums);
    static LabelsQueues createQueues(BWImage& image, BWImage& minimums, std::vector<int>& labels);
    static void watershedLabeling(BWImage& image, BWImage& minimums, std::vector<int>& labels, LabelsQueues& queues);
    static std::vector<int> createColors(BWImage& minimums, std::vector<int>& labels);
    static RGBImage pseudocolorateOnLabels(BWImage& image, std::vector<int>& labels, std::vector<int>& colors);
};


#endif //RAULTESTQT_WATERSHED_HPP
