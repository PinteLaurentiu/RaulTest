//
// Created by laurentiu on 24.06.2020.
//

#ifndef RAULTESTQT_LOGIN_CONTROLLER_HPP
#define RAULTESTQT_LOGIN_CONTROLLER_HPP


#include <QtWidgets/QMainWindow>
#include <memory>
#include <iostream>
#include "../ui/ui_login_form.h"

class LoginController : public QMainWindow
{
Q_OBJECT
public:
    LoginController();
    void setEmail(const std::string& email);

private slots:
    void showPasswordChecked(int state);
    void loginClicked();
    void registerClicked();
private:
    std::unique_ptr<Ui::LoginForm> ui;

private:
    bool validateEmail();
    bool validatePassword();

    void login(const std::string &username, const std::string &password);
};

#endif //RAULTESTQT_LOGIN_CONTROLLER_HPP
