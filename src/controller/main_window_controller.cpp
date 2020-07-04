//
// Created by laurentiu on 22.05.2020.
//

#include <iostream>
#include "main_window_controller.hpp"
#include "load_from_database_dialog_controller.hpp"
#include "login_controller.hpp"
#include "save_in_database_dialog_controller.hpp"
#include "admin_main_window_controller.hpp"
#include "../model/token.hpp"
#include "../transformation/grayscale.hpp"
#include "../transformation/low_pass.hpp"
#include "sensibility_dialog_controller.hpp"
#include "../transformation/high_pass.hpp"
#include "../transformation/histogram_equalization.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QMimeDatabase>
#include <QImageReader>
#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <filesystem>

MainWindowController::MainWindowController() : QMainWindow(nullptr),
    ui(std::make_unique<Ui::MainWindow>()),
    image(QImage()) {

    ui->setupUi(this);
    ui->picture->setScene(&scene);

    connect(ui->exit, &QAction::triggered, &QApplication::closeAllWindows);
    connect(ui->logout, &QAction::triggered, this, &MainWindowController::logoutPressed);
    connect(ui->openFile, &QAction::triggered, this, &MainWindowController::openFilePressed);
    connect(ui->saveFile, &QAction::triggered, this, &MainWindowController::saveFilePressed);
    connect(ui->saveDatabase, &QAction::triggered, this, &MainWindowController::saveDatabasePressed);
    connect(ui->openDatabase, &QAction::triggered, this, &MainWindowController::openDatabasePressed);

    if (TokenStorage::instance().getToken().userDetails.isAdmin()) {
        auto administration = new QAction("Administration", ui->menubar);
        ui->menubar->addAction(administration);
        connect(administration, &QAction::triggered, this, &MainWindowController::administrationPressed);
    }

    addTransformationActions();
}

void MainWindowController::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    showImage();
}

void MainWindowController::addTransformationActions() {
    connect(ui->actionGrayScale, &QAction::triggered, this, &MainWindowController::grayscale);
    connect(ui->actionLowPass, &QAction::triggered, this, &MainWindowController::lowpass);
    connect(ui->actionHighPass, &QAction::triggered, this, &MainWindowController::highpass);
    connect(ui->actionHistogramLinear, &QAction::triggered, this, &MainWindowController::histogramLinear);
    connect(ui->actionHistogramAdaptive, &QAction::triggered, this, &MainWindowController::histogramAdaptive);

    rgbActions.push_back(ui->actionGrayScale);
    rgbActions.push_back(ui->actionLowPass);
    rgbActions.push_back(ui->actionHighPass);

    bwActions.push_back(ui->actionLowPass);
    bwActions.push_back(ui->actionHighPass);
    bwActions.push_back(ui->actionHistogramLinear);
    bwActions.push_back(ui->actionHistogramAdaptive);
}

void MainWindowController::logoutPressed() {
    TokenStorage::instance().clearToken();
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
    image = ImageCache(reader.read());
    showImage();
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
    bool status = writer.write(image.getQImage());
    if (!status && writer.error() == QImageWriter::UnsupportedFormatError) {
        writer.setFormat("PNG");
        status = writer.write(image.getQImage());
    }
    if (!status) {
        QMessageBox::critical(this, "Error", "Could not save file!");
    }
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

void MainWindowController::openDatabasePressed() {
    auto dialog = LoadFromDatabaseDialogController(this);
    connect(&dialog, &LoadFromDatabaseDialogController::imageImported, this, &MainWindowController::imageImported);
    dialog.setModal(true);
    dialog.exec();
}

void MainWindowController::imageImported(ImageCache& importedImage) {
    image = std::move(importedImage);
    showImage();
}

void MainWindowController::saveDatabasePressed() {
    auto dialog = SaveInDatabaseDialogController(this, image.getImageDto());
    dialog.setModal(true);
    dialog.exec();
}

void MainWindowController::administrationPressed() {
    auto controller = new AdminMainWindowController();
    controller->show();
    close();
    deleteLater();
}

void MainWindowController::showImage() {
    scene.clear();
    ui->descriptionText->clear();
    ui->nameLabel->clear();
    ui->saveFile->setEnabled(false);
    ui->saveDatabase->setEnabled(false);
    disableTransformations();
    if (!image)
        return;
    auto pixmap = QPixmap::fromImage(image.getQImage()).scaled(ui->picture->size(), Qt::KeepAspectRatio);
    auto offsetX = (ui->picture->width() - pixmap.width()) / 2;
    auto offsetY = (ui->picture->height() - pixmap.height()) / 2;
    scene.addPixmap(pixmap)->setOffset(offsetX, offsetY);
    if (image.hasImageOwner()) {
        std::ostringstream output;
        output << "Uploaded by: " << image.getImageDto().owner->name << std::endl
               << "Description:" << std::endl
               << image.getImageDto().description;
        ui->descriptionText->setText(QString::fromStdString(output.str()));
        std::ostringstream nameLabelBuilder;
        nameLabelBuilder << "Name: " << image.getImageDto().name;
        ui->nameLabel->setText(QString::fromStdString(nameLabelBuilder.str()));
    }
    enableTransformations();
    ui->saveFile->setEnabled(true);
    ui->saveDatabase->setEnabled(true);
}

void MainWindowController::disableTransformations() {
    for (auto& action : rgbActions) {
        action->setEnabled(false);
    }
    for (auto& action : bwActions) {
        action->setEnabled(false);
    }
}

void MainWindowController::enableTransformations() {
    if (image.hasImage() && std::holds_alternative<BWImage>(image.getImage())) {
        for (auto& action : bwActions) {
            action->setEnabled(true);
        }
    } else {
        for (auto& action : rgbActions) {
            action->setEnabled(true);
        }
    }
}

void MainWindowController::grayscale() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<BWImage>(currentImage))
        return;
    image = ImageCache(AnyImage(Grayscale()(std::get<RGBImage>(currentImage))));
    showImage();
}

void MainWindowController::lowpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        image = ImageCache(AnyImage(LowPass(dialog.spinnerValue.value())(std::get<RGBImage>(currentImage))));
    else
        image = ImageCache(AnyImage(LowPass(dialog.spinnerValue.value())(std::get<BWImage>(currentImage))));
    showImage();
}

void MainWindowController::highpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        image = ImageCache(AnyImage(HighPass(dialog.spinnerValue.value())(std::get<RGBImage>(currentImage))));
    else
        image = ImageCache(AnyImage(HighPass(dialog.spinnerValue.value())(std::get<BWImage>(currentImage))));
    showImage();
}

void MainWindowController::histogramLinear() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    auto& bwImage = std::get<BWImage>(currentImage);
    image = ImageCache(AnyImage(HistogramEqualization(false)(std::get<BWImage>(currentImage))));
    showImage();
}

void MainWindowController::histogramAdaptive() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    auto& bwImage = std::get<BWImage>(currentImage);
    image = ImageCache(AnyImage(HistogramEqualization(true)(std::get<BWImage>(currentImage))));
    showImage();
}
