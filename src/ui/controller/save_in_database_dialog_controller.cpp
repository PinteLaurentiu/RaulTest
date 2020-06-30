//
// Created by laurentiu on 29.06.2020.
//

#include <http_client/http_client_builder.hpp>
#include <http_client/http_multi_part_builder.hpp>
#include <iostream>
#include <QtWidgets/QMessageBox>
#include <utility>
#include "save_in_database_dialog_controller.hpp"

SaveInDatabaseDialogController::SaveInDatabaseDialogController(QMainWindow *parent, ImageDto &dto) :
    QDialog(parent), dto(dto), ui(std::make_unique<Ui::SaveInDatabaseDialog>()) {
    ui->setupUi(this);

    connect(ui->nameInput, &QLineEdit::textChanged, this, &SaveInDatabaseDialogController::textChanged);
    connect(ui->backButton, &QPushButton::pressed, this, &SaveInDatabaseDialogController::close);
    connect(ui->saveButton, &QPushButton::pressed, this, &SaveInDatabaseDialogController::savePressed);

    if (dto.id != 0) {
        ui->saveButton->setText("Update");
        ui->nameInput->setText(QString::fromStdString(dto.name));
        ui->descriptionInput->setText(QString::fromStdString(dto.description));
    }
}

void SaveInDatabaseDialogController::textChanged(const QString& newText) {
    ui->saveButton->setEnabled(!newText.trimmed().isEmpty());
}

void SaveInDatabaseDialogController::savePressed() {
    auto oldName = dto.name;
    auto oldDescription = dto.description;
    dto.name = ui->nameInput->text().trimmed().toStdString();
    dto.description = ui->descriptionInput->toPlainText().toStdString();
    HttpClientBuilder().withType(HttpRequestType::POST)
            .withUrl("/authenticated/image")
            .withAuthentication()
            .withMultipartBody()
            .withPart(dto, "imageDto")
            .withPart(dto.imageData, "imageData")
            .endMultipart()
            .onSuccess<void>([this](){
                close();
            }).onError([name = oldName, description = oldDescription, this](std::exception_ptr exceptionPtr) {
                try {
                    std::rethrow_exception(std::move(exceptionPtr));
                } catch (BackendException& exception) {
                    dto.name = name;
                    dto.description = description;
                    QMessageBox::information(this, "Image upload failed", exception.what());
                    close();
                }
            }).execute();
}
