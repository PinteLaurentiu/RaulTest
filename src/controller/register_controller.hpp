//
// Created by laurentiu on 24.06.2020.
//

#ifndef RAULTESTQT_REGISTER_CONTROLLER_HPP
#define RAULTESTQT_REGISTER_CONTROLLER_HPP

#include <QtWidgets/QMainWindow>
#include <memory>
#include "../ui/ui_register_form.h"
#include "../dto/register_dto.hpp"

class RegisterController : public QMainWindow
{
    Q_OBJECT
public:
    RegisterController();

private slots:
    void showPasswordChecked(int state);
    void submitClicked();
    void backClicked(const std::string& email = "");

private:
    std::unique_ptr<Ui::RegisterForm> ui;

private:
    bool validateName();
    bool validateEmail();
    bool validatePassword();
    void sendRegisterRequest(RegisterDto &dto);
};


#endif //RAULTESTQT_REGISTER_CONTROLLER_HPP
