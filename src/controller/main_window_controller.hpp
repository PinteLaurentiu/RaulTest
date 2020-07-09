//
// Created by laurentiu on 22.05.2020.
//

#ifndef RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
#define RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP

#include <QMainWindow>
#include <memory>
#include <QtWidgets/QFileDialog>
#include "../ui/ui_main.h"
#include "../model/image_cache.hpp"
#include "../model/matrix_transformation_type.hpp"
#include "wait_dialog_controller.hpp"
#include "zoom_graphics_view.hpp"

#include <vector>

class MainWindowController : public QMainWindow
{
Q_OBJECT
public:
    MainWindowController();
    ~MainWindowController() override = default;
private:
    std::unique_ptr<Ui::MainWindow> ui;
    ImageCache image;
    ZoomGraphicsView picture;
    QGraphicsScene scene;
    std::vector<QAction*> bwActions;
    std::vector<QAction*> rgbActions;

signals:
    void newImage();
    void saveFailed();

private slots:
    void logoutPressed();
    void openFilePressed();
    void saveFilePressed();
    void saveDatabasePressed();
    void openDatabasePressed();
    void administrationPressed();
    void imageImported(ImageCache& importedImage);
    void undo();
    void redo();
    void clearHistory();
    void returnToOriginal();
    void showSaveFailedMessage();

private slots:
    void grayscale();
    void lowpass();
    void highpass();
    void histogramLinear();
    void histogramAdaptive();
    void applyMatrix(MatrixTransformationType type);
    void applyBidirectionalMatrix(BidirectionalMatrixTransformationType type);
    void inversColors();
    void otsuThresholding();
    void pseudocoloration();
    void erosion();
    void dilation();
    void closing();
    void opening();
    void watershed();
    void gaussianBlur();
    void canny();
    void quick1();
    void quick2();
    void quick3();
    void quick4();
    void quick5();
    void quick6();
    void quick7();

private:
    static void setMimeTypes(QFileDialog &dialog);
    void showImage();
    void addTransformationActions();
    void disableTransformations();
    void enableTransformations();
    void setImage(ImageCache&& cache);

    void updateEnabledStateMenus() const;
};

#endif //RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
