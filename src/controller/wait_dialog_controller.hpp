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
    WaitDialogController(QWidget* parent, std::function<void()>&& callable);
    explicit WaitDialogController(QWidget* parent);
    void requestClose();

signals:
    void doneWork();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    bool timeToClose = false;
    std::unique_ptr<Ui::waitDialog> ui;
};





#endif //RAULTESTQT_WAIT_DIALOG_CONTROLLER_HPP
