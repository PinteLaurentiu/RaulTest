//
// Created by laurentiu on 22.05.2020.
//

#ifndef RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
#define RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP

#include <QMainWindow>
#include <memory>
#include <QtWidgets/QFileDialog>
#include <dto/image_dto.hpp>
#include <model/image.hpp>
#include <dto/owner_dto.hpp>
#include "../ui/ui_main.h"

class MainWindowController : public QMainWindow
{
Q_OBJECT
public:
    MainWindowController();
    ~MainWindowController() override = default;
private:
    std::unique_ptr<Ui::MainWindow> ui;
    QImage qImage;
    std::optional<RGBImage> rgbImage;
    std::optional<ImageDto> imageDto;
    std::optional<OwnerDto> imageOwner;
    QGraphicsScene scene;

private slots:
    void logoutPressed();
    void openFilePressed();
    void saveFilePressed();
    void saveDatabasePressed();
    void openDatabasePressed();
    void administrationPressed();
    void imageImported(ImageDto image, OwnerDto owner);

private:
    static void setMimeTypes(QFileDialog &dialog);
    void resizeEvent(QResizeEvent* event) override;
    void showImage();
    RGBImage& getRGBImage();
    ImageDto& getImageDto();

};


#endif //RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
