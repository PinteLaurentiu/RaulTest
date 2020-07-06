//
// Created by laurentiu on 24.06.2020.
//

#include <QtWidgets/QMessageBox>
#include "register_controller.hpp"
#include "login_controller.hpp"
#include "../http_client/http_client_builder.hpp"
#include "../exceptions/backend_exception.hpp"
#include <regex>
#include <utility>

RegisterController::RegisterController() : QMainWindow(nullptr), ui(std::make_unique<Ui::RegisterForm>()) {
    ui->setupUi(this);
    connect(ui->showPasswordCheckBox, &QCheckBox::stateChanged, this, &RegisterController::showPasswordChecked);
    connect(ui->submitButton, &QCheckBox::clicked, this, &RegisterController::submitClicked);
    connect(ui->goBackButton, &QCheckBox::clicked, [this](){backClicked();});
}

void RegisterController::showPasswordChecked(int state) {
    if (state != Qt::CheckState::Checked) {
        ui->passwordCheckField->setEchoMode(QLineEdit::Password);
        ui->passwordField->setEchoMode(QLineEdit::Password);
    } else {
        ui->passwordCheckField->setEchoMode(QLineEdit::Normal);
        ui->passwordField->setEchoMode(QLineEdit::Normal);
    }
}

void RegisterController::submitClicked() {
    if (!validateName()) {
        ui->passwordField->clear();
        ui->passwordCheckField->clear();
        return;
    }
    if (!validateEmail()) {
        ui->passwordField->clear();
        ui->passwordCheckField->clear();
        return;
    }
    if (!validatePassword()) {
        ui->passwordField->clear();
        ui->passwordCheckField->clear();
        return;
    }
    RegisterDto dto;
    dto.name = ui->nameField->text().trimmed().toStdString();
    dto.email = ui->emailField->text().trimmed().toStdString();
    dto.password = ui->passwordField->text().toStdString();
    sendRegisterRequest(dto);
}

bool RegisterController::validateName() {
    if (ui->nameField->text().trimmed().isEmpty()) {
        QMessageBox::information(this, "Missing name", "Please provide a valid name");
        return false;
    }
    return true;
}

bool RegisterController::validateEmail() {
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

bool RegisterController::validatePassword() {
    auto qPassword = ui->passwordField->text();
    if (qPassword.trimmed().isEmpty()) {
        QMessageBox::information(this, "Missing password", "Please provide a valid password");
        return false;
    }
    auto password = qPassword.toStdString();
    if (password.size() < 6) {
        QMessageBox::information(this, "Invalid password", "Password must be at least 6 characters long");
        return false;
    }
    if (std::find_if(password.begin(), password.end(), [](char c){return std::isspace(c);}) != password.end()) {
        QMessageBox::information(this, "Invalid password", "Password must not contain space");
        return false;
    }
    if (std::find_if(password.begin(), password.end(), [](char c){return std::isdigit(c);}) == password.end() ||
        std::find_if(password.begin(), password.end(), [](char c){return std::isalpha(c);}) == password.end()) {
        QMessageBox::information(this, "Invalid password", "Password must contain at least one digit and one letter");
        return false;
    }
    if (password != ui->passwordCheckField->text().toStdString()) {
        QMessageBox::information(this, "Invalid password", "Passwords do not match");
        return false;
    }
    return true;
}

void RegisterController::sendRegisterRequest(RegisterDto& dto) {
    HttpClientBuilder().withType(HttpRequestType::POST)
                       .withUrl("/unauthenticated/user")
                       .withBody<RegisterDto>(std::move(dto))
                       .onSuccess<void>([this](){
                           std::ostringstream message;
                           message << "Successfully registered! This account is by default disabled! "
                                   << "To enable your account send an email to documents.raulapp@gmail.com "
                                   << "with your documents that proves your rank as a doctor!";
                           QMessageBox::information(this, "Registered", QString::fromStdString(message.str()));
                           backClicked(ui->emailField->text().toStdString());
                       })
                       .onError([this](std::exception_ptr exceptionPtr){
                           try {
                               std::rethrow_exception(std::move(exceptionPtr));
                           } catch (BackendException& exception) {
                               QMessageBox::information(this, "Registration failed", exception.what());
                           }
                       }).execute();
}

void RegisterController::backClicked(const std::string& email) {
    auto controller = new LoginController();
    controller->setEmail(email);
    controller->show();
    this->close();
    this->deleteLater();
}
