//
// Created by laurentiu on 03.07.2020.
//

#ifndef RAULTESTQT_SENSIBILITY_DIALOG_CONTROLLER_HPP
#define RAULTESTQT_SENSIBILITY_DIALOG_CONTROLLER_HPP

#include <memory>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMainWindow>
#include "../ui/ui_sensibility_dialog.h"

class SensibilityDialogController : public QDialog {
    Q_OBJECT
public:
    explicit SensibilityDialogController(QWidget* parent);
    std::optional<int> spinnerValue;
private slots:
    void applyPressed();

private:
    std::unique_ptr<Ui::sensibilityDialog> ui;
};


#endif //RAULTESTQT_SENSIBILITY_DIALOG_CONTROLLER_HPP
