//
// Created by laurentiu on 22.05.2020.
//

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
#include "../transformation/color_inversion.hpp"
#include "../transformation/gaussian_blur.hpp"
#include "../transformation/canny.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QMimeDatabase>
#include <QImageReader>
#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <filesystem>

MainWindowController::MainWindowController() : QMainWindow(nullptr),
    ui(std::make_unique<Ui::MainWindow>()),
    image(QImage()),
    picture(this) {

    ui->setupUi(this);
    picture.setScene(&scene);
    ui->verticalLayout->addWidget(&picture);

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
    connect(this, &MainWindowController::newImage, this, &MainWindowController::showImage, Qt::QueuedConnection);
    connect(this, &MainWindowController::saveFailed, this, &MainWindowController::showSaveFailedMessage, Qt::QueuedConnection);
    connect(ui->increaseZoom, &QAction::triggered, &picture, &ZoomGraphicsView::increaseZoom);
    connect(ui->decreaseZoom, &QAction::triggered, &picture, &ZoomGraphicsView::decreaseZoom);
    connect(ui->resetZoom, &QAction::triggered, &picture, &ZoomGraphicsView::originalSize);

    if (TokenStorage::instance().getToken().userDetails.isAdmin()) {
        auto administration = new QAction("Administration", ui->menubar);
        ui->menubar->addAction(administration);
        connect(administration, &QAction::triggered, this, &MainWindowController::administrationPressed);
    }

    addTransformationActions();
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
    connect(ui->actionColorInversion, &QAction::triggered, this, &MainWindowController::inversColors);
    connect(ui->actionGaussian, &QAction::triggered, this, &MainWindowController::gaussianBlur);
    connect(ui->actionCanny, &QAction::triggered, this, &MainWindowController::canny);
    connect(ui->actionQuick1, &QAction::triggered, this, &MainWindowController::quick1);
    connect(ui->actionQuick2, &QAction::triggered, this, &MainWindowController::quick2);
    connect(ui->actionQuick3, &QAction::triggered, this, &MainWindowController::quick3);
    connect(ui->actionQuick4, &QAction::triggered, this, &MainWindowController::quick4);
    connect(ui->actionQuick5, &QAction::triggered, this, &MainWindowController::quick5);
    connect(ui->actionQuick6, &QAction::triggered, this, &MainWindowController::quick6);
    connect(ui->actionQuick7, &QAction::triggered, this, &MainWindowController::quick7);

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
    rgbActions.push_back(ui->actionColorInversion);
    rgbActions.push_back(ui->actionGaussian);
    rgbActions.push_back(ui->actionQuick1);
    rgbActions.push_back(ui->actionQuick2);
    rgbActions.push_back(ui->actionQuick3);
    rgbActions.push_back(ui->actionQuick4);
    rgbActions.push_back(ui->actionQuick5);
    rgbActions.push_back(ui->actionQuick6);
    rgbActions.push_back(ui->actionQuick7);

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
    bwActions.push_back(ui->actionColorInversion);
    bwActions.push_back(ui->actionGaussian);
    bwActions.push_back(ui->actionCanny);
    bwActions.push_back(ui->actionQuick1);
    bwActions.push_back(ui->actionQuick2);
    bwActions.push_back(ui->actionQuick3);
    bwActions.push_back(ui->actionQuick4);
    bwActions.push_back(ui->actionQuick5);
    bwActions.push_back(ui->actionQuick6);
    bwActions.push_back(ui->actionQuick7);

    disableTransformations();
}

void MainWindowController::setImage(ImageCache&& cache) {
    if (image)
        ImageHistory::instance().push(std::move(image));
    image = std::move(cache);
    emit newImage();
}

void MainWindowController::undo() {
    image = ImageHistory::instance().popBack(std::move(image));
    emit newImage();
}

void MainWindowController::redo() {
    image = ImageHistory::instance().popFront(std::move(image));
    emit newImage();
}

void MainWindowController::clearHistory() {
    ImageHistory::instance().clear();
    emit newImage();
}

void MainWindowController::returnToOriginal() {
    image = ImageHistory::instance().returnToOriginal(std::move(image));
    emit newImage();
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
    WaitDialogController(this, [this, filename = files[0]] {
        QImageReader reader;
        reader.setFileName(filename);
        ImageHistory::instance().clear();
        picture.originalSize();
        image = ImageCache(QImage());
        setImage(ImageCache(reader.read()));
    }).exec();
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
    WaitDialogController(this, [this, filename = files[0]] {
        QImageWriter writer;
        writer.setFileName(filename);
        bool status = writer.write(image.getQImage());
        if (!status && writer.error() == QImageWriter::UnsupportedFormatError) {
            writer.setFormat("PNG");
            status = writer.write(image.getQImage());
        }
        if (!status) {
            emit saveFailed();
        }
    }).exec();
}

void MainWindowController::showSaveFailedMessage() {
    QMessageBox::critical(this, "Error", "Could not save file!");
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
    picture.originalSize();
    setImage(std::move(importedImage));
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
    ui->saveFile->setEnabled(false);
    ui->saveDatabase->setEnabled(false);
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    ui->clearHistory->setEnabled(false);
    ui->increaseZoom->setEnabled(false);
    ui->decreaseZoom->setEnabled(false);
    ui->resetZoom->setEnabled(false);
    ui->original->setEnabled(false);
    disableTransformations();
    picture.setImage(image);
    enableTransformations();
    ui->saveFile->setEnabled(true);
    ui->saveDatabase->setEnabled(true);
    ui->undo->setEnabled(ImageHistory::instance().hasBack());
    ui->redo->setEnabled(ImageHistory::instance().hasFront());
    ui->clearHistory->setEnabled(ImageHistory::instance().hasFront() || ImageHistory::instance().hasBack());
    ui->increaseZoom->setEnabled(true);
    ui->decreaseZoom->setEnabled(true);
    ui->resetZoom->setEnabled(true);
    ui->original->setEnabled(true);
}

void MainWindowController::disableTransformations() {
    for (auto& action : rgbActions) {
        action->setEnabled(false);
    }
    for (auto& action : bwActions) {
        action->setEnabled(false);
    }
    updateEnabledStateMenus();
}

void MainWindowController::updateEnabledStateMenus() const {
    for (int i = 1; i <= 7; ++i) {
        std::ostringstream buffer;
        buffer << "menu" << i;
        auto menu = findChild<QMenu*>(QString::fromStdString(buffer.str()), Qt::FindChildrenRecursively);
        if (!menu)
            continue;

        menu->setEnabled(false);
        for (auto action : menu->actions()) {
            if (action->isEnabled()) {
                menu->setEnabled(true);
                break;
            }
        }
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
    updateEnabledStateMenus();
}

void MainWindowController::grayscale() {
    WaitDialogController(this, [this] {
        auto &currentImage = image.getImage();
        if (std::holds_alternative<BWImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Grayscale()(std::get<RGBImage>(currentImage)))));
    }).exec();
}

void MainWindowController::lowpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {
        auto &currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            setImage(ImageCache(AnyImage(LowPass(value)(std::get<RGBImage>(currentImage)))));
        else
            setImage(ImageCache(AnyImage(LowPass(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::highpass() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {

    auto& currentImage = image.getImage();
    if (std::holds_alternative<RGBImage>(currentImage))
        setImage(ImageCache(AnyImage(HighPass(value)(std::get<RGBImage>(currentImage)))));
    else
        setImage(ImageCache(AnyImage(HighPass(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::histogramLinear() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(HistogramEqualization(false)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::histogramAdaptive() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(HistogramEqualization(true)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::applyMatrix(MatrixTransformationType type) {
    WaitDialogController(this, [this, type] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            setImage(ImageCache(AnyImage(MatrixTransformation(type)(std::get<RGBImage>(currentImage)))));
        else
            setImage(ImageCache(AnyImage(MatrixTransformation(type)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::applyBidirectionalMatrix(BidirectionalMatrixTransformationType type) {
    WaitDialogController(this, [this, type] {
    auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            setImage(ImageCache(AnyImage(BidirectionalMatrixTransformation(type)(std::get<RGBImage>(currentImage)))));
        else
            setImage(ImageCache(AnyImage(BidirectionalMatrixTransformation(type)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::inversColors() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            setImage(ImageCache(AnyImage(ColorInversion()(std::get<RGBImage>(currentImage)))));
        else
            setImage(ImageCache(AnyImage(ColorInversion()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::otsuThresholding() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(OtsuThresholding()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::pseudocoloration() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Pseudocoloration()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::erosion() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Erosion(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::dilation() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Dilation(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::closing() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Closing(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::opening() {
    auto dialog = SensibilityDialogController(this);
    dialog.exec();
    if (!dialog.spinnerValue)
        return;
    auto value = dialog.spinnerValue.value();
    WaitDialogController(this, [this, value] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Opening(value)(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::watershed() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Watershed()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::gaussianBlur() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            setImage(ImageCache(AnyImage(GaussianBlur()(std::get<RGBImage>(currentImage)))));
        else
            setImage(ImageCache(AnyImage(GaussianBlur()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::canny() {
    WaitDialogController(this, [this] {
        auto& currentImage = image.getImage();
        if (std::holds_alternative<RGBImage>(currentImage))
            return;
        setImage(ImageCache(AnyImage(Canny()(std::get<BWImage>(currentImage)))));
    }).exec();
}

void MainWindowController::quick1() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick2() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick3() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick4() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick5() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick6() {
    WaitDialogController(this, [this] {
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
    }).exec();
}

void MainWindowController::quick7() {
    WaitDialogController(this, [this] {
        BWImage bwImage;
        auto& currentImage = image.getImage();
        std::reference_wrapper<BWImage> imageReference = std::ref(bwImage);
        if (std::holds_alternative<RGBImage>(currentImage)) {
            bwImage = Grayscale()(std::get<RGBImage>(currentImage));
        } else {
            imageReference = std::ref(std::get<BWImage>(currentImage));
        }
        auto t1 = HistogramEqualization(false)(imageReference.get());
        auto t2 = Canny()(t1);
        setImage(ImageCache(AnyImage(std::move(t2))));
    }).exec();
}