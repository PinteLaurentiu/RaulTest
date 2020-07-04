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

private slots:
    void grayscale();
    void lowpass();
    void highpass();
    void histogramLinear();
    void histogramAdaptive();

private:
    static void setMimeTypes(QFileDialog &dialog);
    void resizeEvent(QResizeEvent* event) override;
    void showImage();
    void addTransformationActions();
    void disableTransformations();
    void enableTransformations();

};


#endif //RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
