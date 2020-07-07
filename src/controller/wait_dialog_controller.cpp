//
// Created by laurentiu on 07.07.2020.
//
#include "wait_dialog_controller.hpp"

WaitDialogController::WaitDialogController(QWidget* parent) : QDialog(parent), ui(std::make_unique<Ui::waitDialog>()) {
    ui->setupUi(this);
}

WaitDialogController::WaitDialogController(QWidget* parent, std::function<void()>&& callable) : WaitDialogController(parent) {
    connect(this,
            &WaitDialogController::doneWork,
            this,
            &WaitDialogController::close,
            Qt::ConnectionType::QueuedConnection);
    std::thread([&](){
        callable();
        requestClose();
    }).detach();
}

void WaitDialogController::requestClose() {
    timeToClose = true;
    emit doneWork();
}

void WaitDialogController::closeEvent(QCloseEvent* event) {
    if (!timeToClose)
        event->ignore();
    else
        event->accept();
}

