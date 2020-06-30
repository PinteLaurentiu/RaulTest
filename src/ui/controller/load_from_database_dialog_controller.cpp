////
//// Created by laurentiu on 22.05.2020.
////

#include <http_client/http_client_builder.hpp>
#include <utility>
#include <QtWidgets/QMessageBox>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <src/ui/conversion/qimage_converter.hpp>
#include <exceptions/http_client_exception.hpp>
#include "load_from_database_dialog_controller.hpp"

LoadFromDatabaseDialogController::LoadFromDatabaseDialogController(QWidget* parent) :
    QDialog(parent), ui(std::make_unique<Ui::loadDatabaseDialog>()) {
    ui->setupUi(this);
    ui->imageView->setScene(&scene);
    ui->listView->setModel(&model);

    connect(ui->backButton, &QPushButton::clicked, this, &LoadFromDatabaseDialogController::close);
    connect(ui->deleteButton, &QPushButton::clicked, this, &LoadFromDatabaseDialogController::deletePressed);
    connect(ui->listView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &LoadFromDatabaseDialogController::selectionChanged);
    connect(ui->importButton, &QPushButton::clicked, this, &LoadFromDatabaseDialogController::importPressed);
    populate();
}

void LoadFromDatabaseDialogController::selectionChanged(const QItemSelection& current, const QItemSelection&) {
    ui->deleteButton->setEnabled(false);
    ui->importButton->setEnabled(false);
    if (current.indexes().isEmpty()) {
        return;
    }
    auto& view = views.at(current.indexes()[0].row());
    ui->deleteButton->setEnabled(true);
    ui->importButton->setEnabled(true);
    if (!view.first.imageData.isNull() && view.second.id != 0) {
        showImage(view);
        return;
    }
    loadImageData(view);
}

void LoadFromDatabaseDialogController::loadImageData(ViewType& view) {
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/image/{id}")
        .withUrlParameter("id", std::to_string(view.first.id))
        .withAuthentication()
        .onSuccess([&view = view, this](QByteArray array) {
            auto hash = QCryptographicHash::hash(array, QCryptographicHash::Sha1).toHex().toStdString();
            if (hash != view.first.checksum) {
                throw DownloadCorruptionException();
            }
            view.first.imageData = std::move(array);
            loadImageOwner(view);
        })
        .onError([](std::exception_ptr exceptionPtr){
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
        }).execute();
}

void LoadFromDatabaseDialogController::deletePressed() {
    auto selection = ui->listView->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }
    auto id = views.at(selection.indexes()[0].row()).first.id;
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/image/{id}/delete")
        .withUrlParameter("id", std::to_string(id))
        .withAuthentication()
        .onSuccess<void>([this](){
            populate();
        })
        .onError([](std::exception_ptr exceptionPtr){
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
        })
        .execute();
}

void LoadFromDatabaseDialogController::populate() {
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/image/all")
        .withAuthentication()
        .onSuccess<std::vector<ImageDto>>([this](std::vector<ImageDto> dtos) {
            for (auto viewIter = views.begin(); viewIter != views.end();) {
                auto iter = std::find_if(dtos.begin(), dtos.end(), [&](ImageDto& dto){
                    return dto.id == viewIter->first.id;
                });
                if (iter == dtos.end()) {
                    viewIter = views.erase(viewIter);
                } else {
                    ++viewIter;
                }
            }
            for (auto& view : views) {
                auto iter = std::find_if(dtos.begin(), dtos.end(), [&](ImageDto& dto){
                    return dto.id == view.first.id;
                });
                if (iter != dtos.end()) {
                    dtos.erase(iter);
                }
            }
            for (auto& dto : dtos) {
                views.emplace_back(dto, OwnerDto());
            }
            QStringList list;
            for (auto& view : views) {
                list.push_back(QString::fromStdString(view.first.name));
            }
            model.setStringList(list);
            scene.clear();
            ui->deleteButton->setEnabled(false);
            ui->importButton->setEnabled(false);
        })
        .onError([](std::exception_ptr exceptionPtr) {
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
        }).execute();
}

void LoadFromDatabaseDialogController::importPressed() {
    auto selection = ui->listView->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }
    auto view = views.at(selection.indexes()[0].row());
    if (view.first.imageData.isNull() || view.second.id == 0) {
        return;
    }
    emit imageImported(view.first, view.second);
    close();
}

void LoadFromDatabaseDialogController::showImage(ViewType& pair) {
    std::ostringstream output;
    output << "Uploaded by: " << pair.second.name << std::endl
           << "Description:" << std::endl
           << pair.first.description;
    ui->descriptionText->setText(QString::fromStdString(output.str()));
    auto qImage = QImageConverter()(static_cast<RGBImage>(pair.first));
    auto pixmap = QPixmap::fromImage(qImage).scaled(ui->imageView->size(), Qt::KeepAspectRatio);
    scene.clear();
    auto offsetX = (ui->imageView->width() - pixmap.width()) / 2;
    auto offsetY = (ui->imageView->height() - pixmap.height()) / 2;
    scene.addPixmap(pixmap)->setOffset(offsetX, offsetY);
}

void LoadFromDatabaseDialogController::loadImageOwner(ViewType& view) {
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/user/image/{id}")
        .withUrlParameter("id", std::to_string(view.first.id))
        .withAuthentication()
        .onSuccess<OwnerDto>([&view = view, this](const OwnerDto& name) {
            view.second = name;
            showImage(view);
        })
        .onError([](std::exception_ptr exceptionPtr){
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
        }).execute();
}

