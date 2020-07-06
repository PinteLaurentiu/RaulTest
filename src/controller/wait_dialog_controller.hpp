//
// Created by laurentiu on 06.07.2020.
//

#ifndef RAULTESTQT_WAIT_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_WAIT_DIALOG_CONTROLLER_HPP


#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include <memory>
#include <thread>
#include "../ui/ui_wait_dialog.h"

class WaitDialogController : public QDialog {
Q_OBJECT
public:
    template<typename Callable>
    WaitDialogController(QWidget* parent, Callable&& callable);

signals:
    void doneWork();

protected:
    inline void closeEvent(QCloseEvent* event) override {
        if (!timeToClose)
            event->ignore();
        else
            event->accept();
    }

private:
    bool timeToClose = false;
    std::unique_ptr<Ui::waitDialog> ui;
};

template<typename Callable>
WaitDialogController::WaitDialogController(QWidget *parent, Callable&& callable) : QDialog(parent),
    ui(std::make_unique<Ui::waitDialog>()) {
    ui->setupUi(this);
    connect(this,
            &WaitDialogController::doneWork,
            this,
            &WaitDialogController::close,
            Qt::ConnectionType::QueuedConnection);
    std::thread([&](){
        callable();
        timeToClose = true;
        emit doneWork();
    }).detach();
}


#endif //RAULTESTQT_WAIT_DIALOG_CONTROLLER_HPP
