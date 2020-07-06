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
    QGraphicsScene scene;
    std::vector<QAction*> bwActions;
    std::vector<QAction*> rgbActions;

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

private slots:
    void grayscale();
    void lowpass();
    void highpass();
    void histogramLinear();
    void histogramAdaptive();
    void applyMatrix(MatrixTransformationType type);
    void applyBidirectionalMatrix(BidirectionalMatrixTransformationType type);
    void otsuThresholding();
    void pseudocoloration();
    void erosion();
    void dilation();
    void closing();
    void opening();
    void watershed();
    void quick1();
    void quick2();
    void quick3();
    void quick4();
    void quick5();
    void quick6();

private:
    static void setMimeTypes(QFileDialog &dialog);
    void resizeEvent(QResizeEvent* event) override;
    void showImage();
    void addTransformationActions();
    void disableTransformations();
    void enableTransformations();
    void setImage(ImageCache& cache);
    void setImage(ImageCache&& cache);
};


#endif //RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
