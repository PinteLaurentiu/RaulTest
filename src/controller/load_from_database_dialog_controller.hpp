////
//// Created by laurentiu on 13.06.2020.
////

#ifndef RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP

#include <QDialog>
#include <memory>
#include <QtCore/QStringListModel>
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
    void showImage(ImageCache& image);
    void loadImageData(ImageCache& image);
    void loadImageOwner(ImageCache& image);

signals:
    void imageImported(ImageCache& image);

private:
    std::unique_ptr<Ui::loadDatabaseDialog> ui;
    std::vector<ImageCache> views;
    QGraphicsScene scene;
    QStringListModel model;
};

#endif //RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
