////
//// Created by laurentiu on 13.06.2020.
////

#ifndef RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP

#include <QDialog>
#include <memory>
#include <QtCore/QStringListModel>
#include "../controller/wait_dialog_controller.hpp"
#include "../ui/ui_load_database_dialog.h"
#include "../model/image_cache.hpp"

class LoadFromDatabaseDialogController : public QDialog
{
Q_OBJECT
public:
    explicit LoadFromDatabaseDialogController(QWidget* parent);

private slots:
    void deletePressed();
    void importPressed();
    void selectionChanged(const QItemSelection& current, const QItemSelection&);

private:
    void populate();
    void showImage(std::pair<ImageCache, OwnerDto>& image);
    void loadImageData(std::pair<ImageCache, OwnerDto>& image);
    void loadImageOwner(std::pair<ImageCache, OwnerDto>& image, WaitDialogController* wait);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void imageImported(ImageCache& image);

private:
    std::unique_ptr<Ui::loadDatabaseDialog> ui;
    std::vector<std::pair<ImageCache, OwnerDto>> views;
    QGraphicsScene scene;
    QStringListModel model;
    bool populated = false;
};

#endif //RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
