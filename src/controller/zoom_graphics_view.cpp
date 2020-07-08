//
// Created by laurentiu on 09.07.2020.
//

#include "zoom_graphics_view.hpp"
#include <QResizeEvent>

void ZoomGraphicsView::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    setTransformationAnchor(ViewportAnchor::NoAnchor);
    setResizeAnchor(ViewportAnchor::NoAnchor);
    if (event->delta() > 0) {
        zoom *= 1.25f;
    }
    else {
        zoom /= 1.25f;
    }
    resizeContent();
}

ZoomGraphicsView::ZoomGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ZoomGraphicsView::setImage(ImageCache& image) {
    if (!image)
        return;
    auto pixmap = QPixmap::fromImage(image.getQImage());
    scene()->addPixmap(pixmap);
    scene()->setSceneRect(pixmap.rect());
    contentWidth = static_cast<float>(pixmap.width());
    contentHeight = static_cast<float>(pixmap.height());
    resizeContent();
}

void ZoomGraphicsView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    resizeContent();
}

void ZoomGraphicsView::resizeContent() {
    scaleFactor = std::min(static_cast<float>(width()) / contentWidth,
                                static_cast<float>(height()) / contentHeight) * zoom;
    resetMatrix();
    scale(scaleFactor, scaleFactor);
    Qt::ScrollBarPolicy policy;
    if (zoom < 1.01f) {
        policy = Qt::ScrollBarAlwaysOff;
    } else {
        policy = Qt::ScrollBarAlwaysOn;
    }
    setHorizontalScrollBarPolicy(policy);
    setVerticalScrollBarPolicy(policy);
}

void ZoomGraphicsView::increaseZoom() {
    zoom *= 1.25f;
    resizeContent();
}

void ZoomGraphicsView::decreaseZoom() {
    zoom /= 1.25f;
    resizeContent();
}

void ZoomGraphicsView::originalSize() {
    zoom = 1.f;
    resizeContent();
}
