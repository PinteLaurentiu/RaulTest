////
//// Created by laurentiu on 13.06.2020.
////

#ifndef RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP

#include <QDialog>
#include <memory>
#include <dto/image_dto.hpp>
#include <QtCore/QStringListModel>
#include <dto/owner_dto.hpp>
#include "../ui/ui_load_database_dialog.h"

class LoadFromDatabaseDialogController : public QDialog
{
Q_OBJECT
public:
    explicit LoadFromDatabaseDialogController(QWidget* parent);
    using ViewType = std::pair<ImageDto, OwnerDto>;

private slots:
    void deletePressed();
    void importPressed();
    void selectionChanged(const QItemSelection& current, const QItemSelection&);

private:
    void populate();
    void showImage(ViewType& pair);
    void loadImageData(ViewType& view);
    void loadImageOwner(ViewType& pair);

signals:
    void imageImported(ImageDto image, OwnerDto owner);

private:
    std::unique_ptr<Ui::loadDatabaseDialog> ui;
    std::vector<ViewType> views;
    QGraphicsScene scene;
    QStringListModel model;

};

#endif //RAULTESTQT_LOAD_FROM_DATABASE_DIALOG_CONTROLLER_HPP
