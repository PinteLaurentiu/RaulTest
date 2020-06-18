//
// Created by laurentiu on 22.05.2020.
//

#include "image_loader_controller.hpp"
#include "../conversion/qimage_converter.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <service/image_service.hpp>
#include <QtGui/QImageReader>
#include <QGraphicsPixmapItem>
#include <QtCore/QMimeDatabase>
#include <filesystem>
#include <iostream>
#include <thread>

ImageLoaderController::ImageLoaderController(QWidget* parent) : QDialog(parent),
    ui(std::make_unique<Ui::ImageLoader>()) {
    ui->setupUi(this);
    ui->picture->setScene(&scene);
    ui->listView->setModel(&model);

    connect(ui->searchButton, &QPushButton::clicked, this, &ImageLoaderController::searchPressed);
    connect(ui->pathText, &QLineEdit::textChanged, this, &ImageLoaderController::pathChanged);
    connect(ui->listView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &ImageLoaderController::selectionChanged);
    connect(this, &ImageLoaderController::loadImage,
            this, &ImageLoaderController::imageLoaded,
            Qt::ConnectionType::BlockingQueuedConnection);
    connect(this, &ImageLoaderController::showDBError,
            this, &ImageLoaderController::errorShown,
            Qt::ConnectionType::BlockingQueuedConnection);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ImageLoaderController::deletePressed);

    populate();
}

void ImageLoaderController::searchPressed() {
    auto fileNames = getFileNames();
    if (fileNames.empty()) {
        QMessageBox::warning(this, "Invalid paths", "Invalid files selected!");
        return;
    }

    std::thread([this](std::vector<std::string> fileNames) {
        QImageConverter converter;
        QImageReader reader;
        for (auto &fileName : fileNames) {
            try {
                reader.setFileName(QString::fromStdString(fileName));
                auto qImage = reader.read();
                auto image = converter(qImage);
                auto bwImage = static_cast<BWImage>(image);
                ImageService::instance().saveImage(bwImage, ui->positive->isChecked());
            } catch (BackendException& ex) {
                emit showDBError(ex);
            }
        }
        populate();
    }, std::move(fileNames)).detach();
}

std::vector<std::string> ImageLoaderController::getFileNames() {
    if (ui->pathText->text().isEmpty()) {
        browse();
    }
    if (!pathIsValid()) {
        return std::vector<std::string>();
    }
    std::vector<std::string> value;
    for (auto& file : ui->pathText->text().split(";", QString::SkipEmptyParts)) {
        value.push_back(file.toStdString());
    }
    return value;
}

void ImageLoaderController::browse() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    setMimeTypes(dialog);
    if (dialog.exec() == QDialog::Rejected) {
        ui->pathText->setText(QString());
        return;
    }
    auto files = dialog.selectedFiles();
    if (files.isEmpty())
        ui->pathText->setText(QString());
    else
        ui->pathText->setText(files.join(';'));
}

void ImageLoaderController::setMimeTypes(QFileDialog& dialog) {
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

void ImageLoaderController::pathChanged(const QString& text) {
    if (text.isEmpty()) {
        ui->searchButton->setEnabled(true);
        ui->searchButton->setText("Browse...");
        return;
    }
    ui->searchButton->setText("Load");
    ui->searchButton->setEnabled(pathIsValid());
}

bool ImageLoaderController::pathIsValid() {
    auto text = ui->pathText->text();
    if (text.isEmpty()) {
        return false;
    }
    auto paths = text.split(";", QString::SkipEmptyParts);
    if (paths.size() == 0) {
        return false;
    }
    for (auto& path : paths) {
        if (!std::filesystem::exists(std::filesystem::path(path.toStdString()))) {
            return false;
        }
    }
    return true;
}

void ImageLoaderController::selectionChanged(const QItemSelection& current, const QItemSelection&) {
    ui->deleteButton->setEnabled(false);
    if (current.indexes().isEmpty()) {
        return;
    }
    auto id = views.at(current.indexes()[0].row()).id;
    ui->deleteButton->setEnabled(true);
    showWaitDialog();
    std::cout << std::this_thread::get_id() << std::endl;
    std::thread([this](long id) {
        std::cout << std::this_thread::get_id() << std::endl;
        auto image = ImageService::instance().get(id);
        QImageConverter converter;
        auto qImage = converter(static_cast<RGBImage>(image));
        auto pixmap = QPixmap::fromImage(qImage);
        pixmap = pixmap.scaled(ui->picture->size(), Qt::KeepAspectRatio);
        emit loadImage(pixmap);
    }, id).detach();
}

void ImageLoaderController::imageLoaded(QPixmap pixmap) {
    scene.clear();
    auto offsetX = (ui->picture->width() - pixmap.width()) / 2;
    auto offsetY = (ui->picture->height() - pixmap.height()) / 2;
    scene.addPixmap(pixmap)->setOffset(offsetX, offsetY);
    hideWaitDialog();
}

void ImageLoaderController::showWaitDialog() {
    waitDialog = std::make_unique<QDialog>(this);
    waitDialog->setWindowTitle("Working");
    QLabel* label = new QLabel(waitDialog.get());
    label->setText("Please wait ...");
    QHBoxLayout* layout = new QHBoxLayout();
    waitDialog->setLayout(layout);
    waitDialog->setModal(true);
    waitDialog->setFixedSize(150, 30);
    waitDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    waitDialog->show();
}

void ImageLoaderController::hideWaitDialog() {
    waitDialog.reset(nullptr);
}

void ImageLoaderController::errorShown(BackendException exception) {
    QMessageBox::warning(this, "Error", exception.what());
}

void ImageLoaderController::deletePressed() {
    auto selection = ui->listView->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }
    auto id = views.at(selection.indexes()[0].row()).id;
    ImageService::instance().deleteImage(id);
    populate();
}

void ImageLoaderController::populate() {
    views = ImageService::instance().views();
    QStringList list;
    for (auto view : views) {
        list.push_back(QString::fromStdString(makeText(view)));
    }
    model.setStringList(list);
    scene.clear();
    ui->deleteButton->setEnabled(false);
}

std::string ImageLoaderController::makeText(const ImageInformation& view) {
    std::ostringstream output;
    std::string result = view.result ? "positive" : "negative";
    output << view.id << " - " << view.width << "x" << view.height << " - " << result;
    return output.str();
}
