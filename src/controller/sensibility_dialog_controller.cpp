//
// Created by laurentiu on 03.07.2020.
//

#include <QtWidgets/QMessageBox>
#include "sensibility_dialog_controller.hpp"

SensibilityDialogController::SensibilityDialogController(QWidget *parent) : QDialog(parent),
    ui(std::make_unique<Ui::sensibilityDialog>()) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &SensibilityDialogController::applyPressed);
    ui->spinBox->setValue(3);
}

void SensibilityDialogController::applyPressed() {
    auto value = ui->spinBox->value();
    if (value < 2 || value > 19) {
        QMessageBox::warning(this, "Invalid value", "Value must be between 2 and 19!");
        return;
    }
    spinnerValue = value;
    this->close();
}
