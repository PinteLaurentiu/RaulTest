//
// Created by laurentiu on 29.06.2020.
//


#include <iostream>
#include <QtWidgets/QMessageBox>
#include <utility>
#include "save_in_database_dialog_controller.hpp"
#include "../http_client/http_client_builder.hpp"
#include "../http_client/http_multi_part_builder.hpp"

SaveInDatabaseDialogController::SaveInDatabaseDialogController(QMainWindow *parent, ImageDto &dto) :
    QDialog(parent), dto(dto), ui(std::make_unique<Ui::SaveInDatabaseDialog>()) {
    ui->setupUi(this);

    connect(ui->nameInput, &QLineEdit::textChanged, this, &SaveInDatabaseDialogController::textChanged);
    connect(ui->backButton, &QPushButton::pressed, this, &SaveInDatabaseDialogController::close);
    connect(ui->saveButton, &QPushButton::pressed, this, &SaveInDatabaseDialogController::savePressed);
}

void SaveInDatabaseDialogController::textChanged(const QString& newText) {
    ui->saveButton->setEnabled(!newText.trimmed().isEmpty());
}

void SaveInDatabaseDialogController::savePressed() {
    auto name = dto.name;
    auto description = dto.description;
    dto.name = ui->nameInput->text().trimmed().toStdString();
    dto.description = ui->descriptionInput->toPlainText().toStdString();
    dto.id = 0;
    auto wait = new WaitDialogController(this);
    wait->setModal(true);
    wait->show();
    HttpClientBuilder().withType(HttpRequestType::POST)
            .withUrl("/authenticated/image")
            .withAuthentication()
            .withMultipartBody()
            .withPart(dto, "imageDto")
            .withPart(dto.imageData, "imageData")
            .endMultipart()
            .onSuccess<void>([this, wait](){
                wait->close();
                delete wait;
                close();
            }).onError([name, description, this, wait](std::exception_ptr exceptionPtr) {
                try {
                    std::rethrow_exception(std::move(exceptionPtr));
                } catch (BackendException& exception) {
                    dto.name = name;
                    dto.description = description;
                    QMessageBox::information(this, "Image upload failed", exception.what());
                    wait->close();
                    delete wait;
                    close();
                }
            }).execute();
}
