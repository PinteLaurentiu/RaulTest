//
// Created by laurentiu on 29.06.2020.
//

#ifndef RAULTESTQT_SAVE_IN_DATABASE_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_SAVE_IN_DATABASE_DIALOG_CONTROLLER_HPP


#include <QtWidgets/QDialog>
#include "main_window_controller.hpp"
#include "../ui/ui_save_database_dialog.h"

class SaveInDatabaseDialogController : public QDialog {
Q_OBJECT
public:
    SaveInDatabaseDialogController(QMainWindow* parent, ImageDto& dto);

private slots:
    void textChanged(const QString& newText);
    void savePressed();
private:
    ImageDto& dto;
    std::unique_ptr<Ui::SaveInDatabaseDialog> ui;

};


#endif //RAULTESTQT_SAVE_IN_DATABASE_DIALOG_CONTROLLER_HPP
