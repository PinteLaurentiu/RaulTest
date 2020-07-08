//
// Created by laurentiu on 09.07.2020.
//

#ifndef RAULTESTQT_ZOOM_GRAPHICS_VIEW_HPP
#define RAULTESTQT_ZOOM_GRAPHICS_VIEW_HPP


#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include "../model/image_cache.hpp"

class ZoomGraphicsView : public QGraphicsView {
public:
    explicit ZoomGraphicsView(QWidget* parent);
    void setImage(ImageCache& image);
    void increaseZoom();
    void decreaseZoom();
    void originalSize();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    float contentWidth;
    float contentHeight;
    float zoom = 1;
    float scaleFactor = 0;

    void resizeContent();
};


#endif //RAULTESTQT_ZOOM_GRAPHICS_VIEW_HPP
