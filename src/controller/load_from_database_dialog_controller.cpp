////
//// Created by laurentiu on 22.05.2020.
////

#include "../http_client/http_client_builder.hpp"
#include <utility>
#include <QtWidgets/QMessageBox>
#include <QGraphicsPixmapItem>
#include <iostream>
#include "load_from_database_dialog_controller.hpp"
#include "../exceptions/http_client_exception.hpp"
#include "../model/token.hpp"

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
}

void LoadFromDatabaseDialogController::populate() {
    auto wait = new WaitDialogController(this);
    wait->setModal(true);
    wait->show();
    HttpClientBuilder().withType(HttpRequestType::GET)
            .withUrl("/authenticated/image/all")
            .withAuthentication()
            .onSuccess<std::vector<ImageDto>>([this, wait](std::vector<ImageDto> dtos) {
                for (auto viewIter = views.begin(); viewIter != views.end();) {
                    auto iter = std::find_if(dtos.begin(), dtos.end(), [&](ImageDto& dto){
                        return dto.id == viewIter->first.getImageDto().id;
                    });
                    if (iter == dtos.end()) {
                        viewIter = views.erase(viewIter);
                    } else {
                        ++viewIter;
                    }
                }
                for (auto& view : views) {
                    auto iter = std::find_if(dtos.begin(), dtos.end(), [&](ImageDto& dto){
                        return dto.id == view.first.getImageDto().id;
                    });
                    if (iter != dtos.end()) {
                        dtos.erase(iter);
                    }
                }
                for (auto& dto : dtos) {
                    views.emplace_back(std::move(dto), OwnerDto());
                }
                QStringList list;
                for (auto& view : views) {
                    list.push_back(QString::fromStdString(view.first.getImageDto().name));
                }
                model.setStringList(list);
                scene.clear();
                ui->descriptionText->clear();
                ui->deleteButton->setEnabled(false);
                ui->importButton->setEnabled(false);
                wait->close();
                delete wait;
            })
            .onError([wait](std::exception_ptr exceptionPtr) {
                try {
                    std::rethrow_exception(std::move(exceptionPtr));
                } catch (BackendException& exception) {
                    QMessageBox::warning(nullptr, "Error", exception.what());
                }
                wait->close();
                delete wait;
            }).execute();
}

void LoadFromDatabaseDialogController::selectionChanged(const QItemSelection& current, const QItemSelection&) {
    ui->deleteButton->setEnabled(false);
    ui->importButton->setEnabled(false);
    if (current.indexes().isEmpty()) {
        return;
    }
    auto& view = views.at(current.indexes()[0].row());
    if (!view.first.getImageDto().imageData.isNull() && view.second.id != 0) {
        showImage(view);
        return;
    }
    loadImageData(view);
}

void LoadFromDatabaseDialogController::loadImageData(std::pair<ImageCache, OwnerDto>& view) {
    auto wait = new WaitDialogController(this);
    wait->setModal(true);
    wait->show();
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/image/{id}")
        .withUrlParameter("id", std::to_string(view.first.getImageDto().id))
        .withAuthentication()
        .onSuccess([&view = view, this, wait](QByteArray array) {
            auto hash = QCryptographicHash::hash(array, QCryptographicHash::Sha1).toHex().toStdString();
            if (hash != view.first.getImageDto().checksum) {
                throw DownloadCorruptionException();
            }
            view.first.getImageDto().imageData = std::move(array);
            loadImageOwner(view, wait);
        })
        .onError([this, wait](std::exception_ptr exceptionPtr){
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
            wait->close();
            delete wait;
            populate();
        }).execute();
}

void LoadFromDatabaseDialogController::deletePressed() {
    auto selection = ui->listView->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }
    auto id = views.at(selection.indexes()[0].row()).first.getImageDto().id;
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

void LoadFromDatabaseDialogController::importPressed() {
    auto selection = ui->listView->selectionModel()->selection();
    if (selection.isEmpty()) {
        return;
    }
    auto& view = views.at(selection.indexes()[0].row());
    if (view.first.getImageDto().imageData.isNull()) {
        return;
    }
    emit imageImported(view.first);
    close();
}

void LoadFromDatabaseDialogController::showImage(std::pair<ImageCache, OwnerDto>& view) {
    std::ostringstream output;
    output << "Uploaded by: " << view.second.name << std::endl
           << "Description:" << std::endl
           << view.first.getImageDto().description;
    ui->descriptionText->setText(QString::fromStdString(output.str()));
    auto pixmap = QPixmap::fromImage(view.first.getQImage()).scaled(ui->imageView->size(), Qt::KeepAspectRatio);
    scene.clear();
    auto offsetX = (ui->imageView->width() - pixmap.width()) / 2;
    auto offsetY = (ui->imageView->height() - pixmap.height()) / 2;
    scene.addPixmap(pixmap)->setOffset(offsetX, offsetY);
    if (view.second.id == TokenStorage::instance().getToken().userDetails.id) {
        ui->deleteButton->setEnabled(true);
    }
    ui->importButton->setEnabled(true);
}

void LoadFromDatabaseDialogController::loadImageOwner(std::pair<ImageCache, OwnerDto>& image, WaitDialogController* wait) {
    HttpClientBuilder().withType(HttpRequestType::GET)
        .withUrl("/authenticated/user/image/{id}")
        .withUrlParameter("id", std::to_string(image.first.getImageDto().id))
        .withAuthentication()
        .onSuccess<OwnerDto>([&view = image, this, wait](const OwnerDto& owner) {
            view.second = owner;
            showImage(view);
            wait->close();
            delete wait;
        })
        .onError([wait](std::exception_ptr exceptionPtr){
            try {
                std::rethrow_exception(std::move(exceptionPtr));
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
            wait->close();
            delete wait;
        }).execute();
}

void LoadFromDatabaseDialogController::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    if (!populated)
        populate();
}

