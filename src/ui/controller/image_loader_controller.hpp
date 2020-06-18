//
// Created by laurentiu on 13.06.2020.
//

#ifndef RAULTESTQT_IMAGE_LOADER_CONTROLLER_HPP
#define RAULTESTQT_IMAGE_LOADER_CONTROLLER_HPP

#include <QMainWindow>
#include <memory>
#include <QtWidgets/QFileDialog>
#include <QtCore/QStringListModel>
#include <model/image_info.hpp>
#include <QtWidgets/QMessageBox>
#include <exceptions/backend_exception.hpp>
#include "../ui/ui_image_loader.h"

class ImageLoaderController : public QDialog
{
Q_OBJECT
public:
    explicit ImageLoaderController(QWidget* parent = nullptr);
    ~ImageLoaderController() override = default;

private slots:
    void searchPressed();
    void selectionChanged(const QItemSelection& current, const QItemSelection&);
    void deletePressed();
    void pathChanged(const QString& text);
    void imageLoaded(QPixmap pixmap);
    void errorShown(BackendException exception);

signals:
    void loadImage(QPixmap pixmap);
    void showDBError(BackendException exception);

private:
    std::vector<std::string> getFileNames();
    void browse();
    void setMimeTypes(QFileDialog& dialog);
    bool pathIsValid();
    void populate();
    std::string makeText(const ImageInformation& view);
    void showWaitDialog();
    void hideWaitDialog();

private:
    std::unique_ptr<Ui::ImageLoader> ui;
    std::vector<ImageInformation> views;
    QGraphicsScene scene;
    QStringListModel model;
    std::unique_ptr<QDialog> waitDialog;
};

#endif //RAULTESTQT_IMAGE_LOADER_CONTROLLER_HPP
