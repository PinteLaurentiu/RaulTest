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
#include "../transformation/matrix_transformation.hpp"
#include "../model/image_history.hpp"
#include "../transformation/bidirectional_matrix_transformation.hpp"
#include "../transformation/otsu_thresholding.hpp"
#include "../transformation/pseudocoloration.hpp"
#include "../transformation/erosion.hpp"
#include "../transformation/dilation.hpp"
#include "../transformation/closing.hpp"
#include "../transformation/opening.hpp"
#include "../transformation/watershed.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QMimeDatabase>
#include <QImageReader>
#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <filesystem>
#include <queue>
#include <random>

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
    connect(ui->undo, &QAction::triggered, this, &MainWindowController::undo);
    connect(ui->redo, &QAction::triggered, this, &MainWindowController::redo);
    connect(ui->clearHistory, &QAction::triggered, this, &MainWindowController::clearHistory);
    connect(ui->original, &QAction::triggered, this, &MainWindowController::returnToOriginal);

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
    connect(ui->actionLaplacian, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::Laplacian); });
    connect(ui->actionDiagonalLaplacian, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::DiagonalLaplacian); });
    connect(ui->actionHLaplacian, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::HorizontalLaplacian); });
    connect(ui->actionVLaplacian, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::VerticalLaplacian); });
    connect(ui->actionHPrewitt, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::HorizontalPrewitt); });
    connect(ui->actionVPrewitt, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::VerticalPrewitt); });
    connect(ui->actionHSobel, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::HorizontalSobel); });
    connect(ui->actionVSobel, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::VerticalSobel); });
    connect(ui->actionHKirsch, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::HorizontalKirsch); });
    connect(ui->actionVKirsch, &QAction::triggered, [this]{ applyMatrix(MatrixTransformationType::VerticalKirsch); });
    connect(ui->actionBLaplacian, &QAction::triggered, [this]{ applyBidirectionalMatrix(BidirectionalMatrixTransformationType::Laplacian); });
    connect(ui->actionBPrewitt, &QAction::triggered, [this]{ applyBidirectionalMatrix(BidirectionalMatrixTransformationType::Prewitt); });
    connect(ui->actionBSobel, &QAction::triggered, [this]{ applyBidirectionalMatrix(BidirectionalMatrixTransformationType::Sobel); });
    connect(ui->actionBKirsch, &QAction::triggered, [this]{ applyBidirectionalMatrix(BidirectionalMatrixTransformationType::Kirsch); });
    connect(ui->actionOtsuThreshold, &QAction::triggered, this, &MainWindowController::otsuThresholding);
    connect(ui->actionPseudocoloration, &QAction::triggered, this, &MainWindowController::pseudocoloration);
    connect(ui->actionErosion, &QAction::triggered, this, &MainWindowController::erosion);
    connect(ui->actionDilation, &QAction::triggered, this, &MainWindowController::dilation);
    connect(ui->actionClosing, &QAction::triggered, this, &MainWindowController::closing);
    connect(ui->actionOpening, &QAction::triggered, this, &MainWindowController::opening);
    connect(ui->actionWatershed, &QAction::triggered, this, &MainWindowController::watershed);
    connect(ui->actionQuick1, &QAction::triggered, this, &MainWindowController::quick1);
    connect(ui->actionQuick2, &QAction::triggered, this, &MainWindowController::quick2);
    connect(ui->actionQuick3, &QAction::triggered, this, &MainWindowController::quick3);
    connect(ui->actionQuick4, &QAction::triggered, this, &MainWindowController::quick4);
    connect(ui->actionQuick5, &QAction::triggered, this, &MainWindowController::quick5);
    connect(ui->actionQuick6, &QAction::triggered, this, &MainWindowController::quick6);

    rgbActions.push_back(ui->actionGrayScale);
    rgbActions.push_back(ui->actionLowPass);
    rgbActions.push_back(ui->actionHighPass);
    rgbActions.push_back(ui->actionLaplacian);
    rgbActions.push_back(ui->actionDiagonalLaplacian);
    rgbActions.push_back(ui->actionHLaplacian);
    rgbActions.push_back(ui->actionVLaplacian);
    rgbActions.push_back(ui->actionHPrewitt);
    rgbActions.push_back(ui->actionVPrewitt);
    rgbActions.push_back(ui->actionHSobel);
    rgbActions.push_back(ui->actionVSobel);
    rgbActions.push_back(ui->actionHKirsch);
    rgbActions.push_back(ui->actionVKirsch);
    rgbActions.push_back(ui->actionBLaplacian);
    rgbActions.push_back(ui->actionBPrewitt);
    rgbActions.push_back(ui->actionBSobel);
    rgbActions.push_back(ui->actionBKirsch);

    bwActions.push_back(ui->actionLowPass);
    bwActions.push_back(ui->actionHighPass);
    bwActions.push_back(ui->actionHistogramLinear);
    bwActions.push_back(ui->actionHistogramAdaptive);
    bwActions.push_back(ui->actionLaplacian);
    bwActions.push_back(ui->actionDiagonalLaplacian);
    bwActions.push_back(ui->actionHLaplacian);
    bwActions.push_back(ui->actionVLaplacian);
    bwActions.push_back(ui->actionHPrewitt);
    bwActions.push_back(ui->actionVPrewitt);
    bwActions.push_back(ui->actionHSobel);
    bwActions.push_back(ui->actionVSobel);
    bwActions.push_back(ui->actionHKirsch);
    bwActions.push_back(ui->actionVKirsch);
    bwActions.push_back(ui->actionBLaplacian);
    bwActions.push_back(ui->actionBPrewitt);
    bwActions.push_back(ui->actionBSobel);
    bwActions.push_back(ui->actionBKirsch);
    bwActions.push_back(ui->actionOtsuThreshold);
    bwActions.push_back(ui->actionPseudocoloration);
    bwActions.push_back(ui->actionErosion);
    bwActions.push_back(ui->actionDilation);
    bwActions.push_back(ui->actionClosing);
    bwActions.push_back(ui->actionOpening);
    bwActions.push_back(ui->actionWatershed);
}

void MainWindowController::setImage(ImageCache& cache) {
    setImage(std::move(cache));
}

void MainWindowController::setImage(ImageCache&& cache) {
    if (image)
        ImageHistory::instance().push(std::move(image));
    image = std::move(cache);
    showImage();
}

void MainWindowController::undo() {
    image = ImageHistory::instance().popBack(std::move(image));
    showImage();
}

void MainWindowController::redo() {
    image = ImageHistory::instance().popFront(std::move(image));
    showImage();
}

void MainWindowController::clearHistory() {
    ImageHistory::instance().clear();
    showImage();
}


void MainWindowController::returnToOriginal() {
    image = ImageHistory::instance().returnToOriginal(std::move(image));
    showImage();
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
    ImageHistory::instance().clear();
    image = ImageCache(QImage());
    setImage(ImageCache(reader.read()));
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
    ImageHistory::instance().clear();
    image = ImageCache(QImage());
    setImage(importedImage);
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
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    ui->clearHistory->setEnabled(false);
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
    ui->undo->setEnabled(ImageHistory::instance().hasBack());
    ui->redo->setEnabled(ImageHistory::instance().hasFront());
    ui->clearHistory->setEnabled(ImageHistory::instance().hasFront() || ImageHistory::instance().hasBack());
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
    setImage(ImageCache(AnyImage(Grayscale()(std::get<RGBImage>(currentImage)))));
}

void MainWindowController::lowpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        setImage(ImageCache(AnyImage(LowPass(dialog.spinnerValue.value())(std::get<RGBImage>(currentImage)))));
    else
        setImage(ImageCache(AnyImage(LowPass(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::highpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        setImage(ImageCache(AnyImage(HighPass(dialog.spinnerValue.value())(std::get<RGBImage>(currentImage)))));
    else
        setImage(ImageCache(AnyImage(HighPass(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::histogramLinear() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(HistogramEqualization(false)(std::get<BWImage>(currentImage)))));
}

void MainWindowController::histogramAdaptive() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(HistogramEqualization(true)(std::get<BWImage>(currentImage)))));
}

void MainWindowController::applyMatrix(MatrixTransformationType type) {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        setImage(ImageCache(AnyImage(MatrixTransformation(type)(std::get<RGBImage>(currentImage)))));
    else
        setImage(ImageCache(AnyImage(MatrixTransformation(type)(std::get<BWImage>(currentImage)))));
}

void MainWindowController::applyBidirectionalMatrix(BidirectionalMatrixTransformationType type) {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        setImage(ImageCache(AnyImage(BidirectionalMatrixTransformation(type)(std::get<RGBImage>(currentImage)))));
    else
        setImage(ImageCache(AnyImage(BidirectionalMatrixTransformation(type)(std::get<BWImage>(currentImage)))));
}

void MainWindowController::otsuThresholding() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(OtsuThresholding()(std::get<BWImage>(currentImage)))));
}

void MainWindowController::pseudocoloration() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Pseudocoloration()(std::get<BWImage>(currentImage)))));
}

void MainWindowController::erosion() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Erosion(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::dilation() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Dilation(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::closing() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Closing(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::opening() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Opening(dialog.spinnerValue.value())(std::get<BWImage>(currentImage)))));
}

void MainWindowController::watershed() {
    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        return;
    setImage(ImageCache(AnyImage(Watershed()(std::get<BWImage>(currentImage)))));
}

void MainWindowController::quick1() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = LowPass(3)(imageReference.get());
    auto t2 = HighPass(7)(t1);
    auto t3 = HistogramEqualization(false)(t2);
    setImage(ImageCache(AnyImage(std::move(t3))));
}

void MainWindowController::quick2() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = BidirectionalMatrixTransformation(BidirectionalMatrixTransformationType::Kirsch)(imageReference.get());
    auto t2 = LowPass(3)(t1);
    setImage(ImageCache(AnyImage(std::move(t2))));
}
void MainWindowController::quick3() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = HighPass(9)(imageReference.get());
    auto t2 = HistogramEqualization(false)(t1);
    auto t3 = OtsuThresholding()(t2);
    setImage(ImageCache(AnyImage(std::move(t3))));
}
void MainWindowController::quick4() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = HistogramEqualization(false)(imageReference.get());
    auto t2 = Pseudocoloration()(t1);
    setImage(ImageCache(AnyImage(std::move(t2))));
}
void MainWindowController::quick5() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = Opening(5)(imageReference.get());
    auto t2 = OtsuThresholding()(t1);
    auto t3 = Watershed()(t2);
    setImage(ImageCache(AnyImage(std::move(t3))));
}

void MainWindowController::quick6() {
    BWImage bwImage;
    auto& currentImage = image.getImage();
    std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
    if (std::holds_alternative<RGBImage>(currentImage)) {
        bwImage = Grayscale()(std::get<RGBImage>(currentImage));
    } else {
        imageReference = std::ref(std::get<BWImage>(currentImage));
    }
    auto t1 = HistogramEqualization(false)(imageReference.get());
    auto t2 = Pseudocoloration()(t1);
    auto t3 = Grayscale()(t2);
    auto t4 = OtsuThresholding()(t3);
    setImage(ImageCache(AnyImage(std::move(t4))));
}