//
// Created by laurentiu on 22.05.2020.
//

#include <iostream>
#include "main_window_controller.hpp"
#include "load_from_database_dialog_controller.hpp"
#include "login_controller.hpp"
#include "save_in_database_dialog_controller.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <model/token.hpp>
#include <QtCore/QMimeDatabase>
#include <QImageReader>
#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <filesystem>
#include <src/ui/conversion/qimage_converter.hpp>

MainWindowController::MainWindowController() : QMainWindow(nullptr), ui(std::make_unique<Ui::MainWindow>()) {
    ui->setupUi(this);
    ui->picture->setScene(&scene);

    connect(ui->openFile, &QAction::triggered, this, &MainWindowController::openFilePressed);
    connect(ui->exit, &QAction::triggered, this, &MainWindowController::exitPressed);
    connect(ui->logout, &QAction::triggered, this, &MainWindowController::logoutPressed);
    connect(ui->saveFile, &QAction::triggered, this, &MainWindowController::saveFilePressed);
    connect(ui->saveDatabase, &QAction::triggered, this, &MainWindowController::saveDatabasePressed);
    connect(ui->openDatabase, &QAction::triggered, this, &MainWindowController::openDatabasePressed);
}

void MainWindowController::exitPressed() {
    this->close();
}

void MainWindowController::logoutPressed() {
    (new LoginController)->show();
    this->close();
    this->deleteLater();
}

void MainWindowController::openFilePressed() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    setMimeTypes(dialog);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    auto files = dialog.selectedFiles();
    if (files.isEmpty())
        return;
    QImageReader reader;
    reader.setFileName(files[0]);
    qImage = reader.read();
    rgbImage.reset(nullptr);
    selectedImageDto.reset(nullptr);
    resizeAndSetImage();
    ui->saveFile->setEnabled(true);
    ui->saveDatabase->setEnabled(true);
}

void MainWindowController::resizeAndSetImage() {
    if (qImage.isNull())
        return;
    auto pixmap = QPixmap::fromImage(qImage).scaled(ui->picture->size(), Qt::KeepAspectRatio);
    scene.clear();
    auto offsetX = (ui->picture->width() - pixmap.width()) / 2;
    auto offsetY = (ui->picture->height() - pixmap.height()) / 2;
    scene.addPixmap(pixmap)->setOffset(offsetX, offsetY);
}

void MainWindowController::setMimeTypes(QFileDialog& dialog) {
    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = QImageReader::supportedMimeTypes();
    for (auto& mimeTypeName : supportedMimeTypes) {
        mimeTypeFilters.append(mimeTypeName);
    }

    QMimeDatabase mimeDB;
    QStringList allSupportedFormats;
    for (auto& mimeTypeFilter : mimeTypeFilters) {
        QMimeType mimeType = mimeDB.mimeTypeForName(mimeTypeFilter);
        if(mimeType.isValid()) {
            allSupportedFormats.append(mimeType.globPatterns());
        }
    }

    QString allSupportedFormatsFilter = QString("All supported formats (%1)").arg(allSupportedFormats.join(' '));
    dialog.setMimeTypeFilters(mimeTypeFilters);
    QStringList nameFilters = dialog.nameFilters();
    nameFilters.append(allSupportedFormatsFilter);
    dialog.setNameFilters(nameFilters);
    dialog.selectNameFilter(allSupportedFormatsFilter);
}

void MainWindowController::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    resizeAndSetImage();
}

void MainWindowController::saveFilePressed() {
    auto dialog = QFileDialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    setMimeTypes(dialog);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    auto files = dialog.selectedFiles();
    if (files.isEmpty())
        return;
    QImageWriter writer;
    writer.setFileName(files[0]);
    bool status = writer.write(qImage);
    if (!status && writer.error() == QImageWriter::UnsupportedFormatError) {
        writer.setFormat("PNG");
        status = writer.write(qImage);
    }
    if (!status) {
        QMessageBox::critical(this, "Error", "Could not save file!");
    }
}

void MainWindowController::saveDatabasePressed() {
    auto dialog = SaveInDatabaseDialogController(this, getImageDto());
    dialog.setModal(true);
    dialog.exec();
}

ImageDto& MainWindowController::getImageDto() {
    if (!selectedImageDto)
        selectedImageDto = std::make_unique<ImageDto>(getRGBImage());
    return *selectedImageDto;
}

RGBImage& MainWindowController::getRGBImage() {
    auto imageConverter = QImageConverter();
    if (!rgbImage) {
        rgbImage = std::make_unique<RGBImage>(imageConverter(qImage));
    }
    return *rgbImage;
}

void MainWindowController::openDatabasePressed() {
    auto dialog = LoadFromDatabaseDialogController(this);
    connect(&dialog, &LoadFromDatabaseDialogController::imageImported, this, &MainWindowController::imageImported);
    dialog.setModal(true);
    dialog.exec();
}

void MainWindowController::imageImported(ImageDto image, OwnerDto owner) {
    selectedImageDto = std::make_unique<ImageDto>(image);
    rgbImage = std::make_unique<RGBImage>(static_cast<RGBImage>(*selectedImageDto));
    auto imageConverter = QImageConverter();
    qImage = imageConverter(*rgbImage);
    resizeAndSetImage();
    ui->saveFile->setEnabled(true);
    ui->saveDatabase->setEnabled(true);
}

