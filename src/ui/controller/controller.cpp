//
// Created by laurentiu on 22.05.2020.
//

#include <iostream>
#include "controller.hpp"
#include "../conversion/qpixmap_converter.hpp"
#include <QFileDialog>

Controller::Controller() : QMainWindow(nullptr), controller(std::make_unique<Ui::Main>()) {
    controller->setupUi(this);
    controller->picture->setScene(&scene);
    connect(controller->searchButton, &QPushButton::clicked, [this](){
        auto dialog = QFileDialog(this);
        dialog.exec();
        controller->pathText->setText(dialog.selectedFiles().at(0));
    });
    connect(controller->openButton, &QPushButton::clicked, [this](){
        auto converter = QPixmapConverter();
        auto pixels = QPixmap(controller->pathText->text());
        auto m_pixels = converter(pixels);
        auto m_bw_pixels = BWImage(m_pixels);
        auto m_bw_as_rgb = RGBImage(m_bw_pixels);
        auto bw_pixels = converter(m_bw_as_rgb);
        scene.addPixmap(bw_pixels);
//        OpenCLBase().main();
    });
    connect(controller->pathText, &QLineEdit::textChanged, [this](const QString& text){
        controller->openButton->setDisabled(text.isEmpty());
    });
}