//
// Created by laurentiu on 22.05.2020.
//

#include <iostream>
#include "main_window_controller.hpp"
#include "../conversion/qimage_converter.hpp"
#include "image_loader_controller.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <service/image_service.hpp>
#include <db/mysql_connection_pool.hpp>

MainWindowController::MainWindowController() : QMainWindow(nullptr), ui(std::make_unique<Ui::Main>()) {
    ui->setupUi(this);
    ui->picture->setScene(&scene);

    if constexpr (hideImageLoader) {
        auto button = ui->openImageLoaderButton;
        ui->centralwidget->layout()->removeWidget(ui->openImageLoaderButton);
        delete button;
    } else {
        connect(ui->openImageLoaderButton,
                &QPushButton::clicked,
                this,
                &MainWindowController::openImageLoaderButtonPushed);
    }

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindowController::searchButtonPushed);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindowController::openButtonPushed);
    connect(ui->pathText, &QLineEdit::textChanged, this, &MainWindowController::pathTextChanged);
}

void MainWindowController::openImageLoaderButtonPushed() {
    auto& x = MysqlConnectionPool::instance();
//    this->hide();
//    ImageLoaderController controller(this);
//    controller.exec();
//    this->show();
}

void MainWindowController::openButtonPushed() {
//        try {
//            auto converter = QImageConverter();
//            auto pixels = QPixmap(controller->pathText->text());
//            auto m_pixels = converter(pixels);
//            auto m_bw_pixels = BWImage(m_pixels);
//            ImageService::instance().saveImage(m_bw_pixels, true);
//        } catch (BackendException& exception) {
//            QMessageBox::warning(this, "Error", exception.what());
//        }
//        auto m_bw_as_rgb = RGBImage(m_bw_pixels);
//        auto bw_pixels = converter(m_bw_as_rgb);
//        scene.addPixmap(bw_pixels);
}

void MainWindowController::searchButtonPushed() {
    auto dialog = QFileDialog(this);
    dialog.exec();
    ui->pathText->setText(dialog.selectedFiles().at(0));
}

void MainWindowController::pathTextChanged(const QString &text) {
    ui->openButton->setDisabled(text.isEmpty());
}
