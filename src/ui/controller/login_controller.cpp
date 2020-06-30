//
// Created by laurentiu on 24.06.2020.
//

#include <QtWidgets/QMessageBox>
#include "login_controller.hpp"
#include "register_controller.hpp"
#include <regex>
#include <http_client/authorization_request.hpp>
#include <exceptions/backend_exception.hpp>
#include <utility>
#include "admin_main_window_controller.hpp"
#include "load_from_database_dialog_controller.hpp"
#include "main_window_controller.hpp"

LoginController::LoginController() : QMainWindow(nullptr), ui(std::make_unique<Ui::LoginForm>()) {
    ui->setupUi(this);
    connect(ui->showPasswordCheckBox, &QCheckBox::stateChanged, this, &LoginController::showPasswordChecked);
    connect(ui->loginButton, &QCheckBox::clicked, this, &LoginController::loginClicked);
    connect(ui->registerButton, &QCheckBox::clicked, this, &LoginController::registerClicked);
}

void LoginController::showPasswordChecked(int state) {
    if (state != Qt::CheckState::Checked) {
        ui->passwordField->setEchoMode(QLineEdit::Password);
    } else {
        ui->passwordField->setEchoMode(QLineEdit::Normal);
    }
}

void LoginController::loginClicked() {
    if (!validateEmail()) {
        ui->passwordField->clear();
        return;
    }
    if (!validatePassword()) {
        ui->passwordField->clear();
        return;
    }
    login(ui->emailField->text().trimmed().toStdString(), ui->passwordField->text().toStdString());
}

void LoginController::registerClicked() {
    (new RegisterController())->show();
    this->close();
    this->deleteLater();
}

bool LoginController::validateEmail() {
    auto qEmail = ui->emailField->text().trimmed();
    if (qEmail.isEmpty()) {
        QMessageBox::information(this, "Missing email", "Please provide a valid email");
        return false;
    }
    auto email = qEmail.toStdString();
    std::regex emailRegex("([A-Za-z0-9-_]+\\.)*[A-Za-z0-9-_]+@([A-Za-z0-9-_]+\\.)*[A-Za-z0-9-_]+");
    if (!std::regex_match(email, emailRegex)) {
        QMessageBox::information(this, "Invalid email", "Please provide a valid email");
        return false;
    }
    return true;
}

bool LoginController::validatePassword() {
    auto qPassword = ui->passwordField->text();
    if (qPassword.trimmed().isEmpty()) {
        QMessageBox::information(this, "Missing password", "Please provide a valid password");
        return false;
    }
    auto password = qPassword.toStdString();
    if (std::find_if(password.begin(), password.end(), [](char c){return std::isspace(c);}) != password.end()) {
        QMessageBox::information(this, "Invalid password", "Password must not contain space");
        return false;
    }
    return true;
}

void LoginController::login(const std::string &username, const std::string &password) {
    AuthorizationRequest request(username, password);
    request([this](Token token){
        TokenStorage::instance().saveToken(token);
        QMainWindow* controller;
        if (token.userDetails.isAdmin()) {
            controller = new AdminMainWindowController();
        } else {
            controller = new MainWindowController();
        }
        controller->show();
        this->close();
        this->deleteLater();
    }, [this](std::exception_ptr exceptionPtr) {
        try {
            std::rethrow_exception(std::move(exceptionPtr));
        } catch (BackendException& exception) {
            QMessageBox::warning(this, "Error", exception.what());
        }
    });
}
