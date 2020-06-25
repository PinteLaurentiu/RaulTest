//
// Created by laurentiu on 25.06.2020.
//

#include "admin_main_window_controller.hpp"
#include "dto/user_dto.hpp"
#include <http_client/http_client_builder.hpp>
#include <QtWidgets/QMessageBox>
#include <exceptions/backend_exception.hpp>
#include <iostream>
#include <model/token.hpp>
#include <utility>
#include "open_windows_cache.hpp"
#include "login_controller.hpp"

AdminMainWindowController::AdminMainWindowController() : QMainWindow(nullptr),
    ui(std::make_unique<Ui::AdminMainWindow>()) {
    ui->setupUi(this);
    ui->listView->setModel(&model);

    connect(ui->generalistRadioButton, &QRadioButton::toggled, this, &AdminMainWindowController::radioChanged);
    connect(ui->rezidentRadioButton, &QRadioButton::toggled, this, &AdminMainWindowController::radioChanged);
    connect(ui->specialistRadioButton, &QRadioButton::toggled, this, &AdminMainWindowController::radioChanged);
    connect(ui->primarRadioButton, &QRadioButton::toggled, this, &AdminMainWindowController::radioChanged);
    connect(ui->adminCheckBox, &QCheckBox::stateChanged, this, &AdminMainWindowController::checkBoxChanged);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminMainWindowController::deletePressed);

    connect(ui->listView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &AdminMainWindowController::selectionChanged);

    saveAvailable = true;

    populate();
}

void AdminMainWindowController::populate() {
    ui->adminCheckBox->setEnabled(false);
    ui->generalistRadioButton->setEnabled(false);
    ui->rezidentRadioButton->setEnabled(false);
    ui->specialistRadioButton->setEnabled(false);
    ui->primarRadioButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    saveAvailable = false;
    ui->adminCheckBox->setCheckState(Qt::Unchecked);
    ui->unassignedRadioButton->setChecked(true);
    ui->label->setText("Select an user");
    saveAvailable = true;
    HttpClientBuilder()
        .withAuthentication()
        .withType(HttpRequestType::GET)
        .withUrl("/admin/user/all")
        .onSuccess<std::vector<UserDto>>([this](const std::vector<UserDto>& newDtos) {
            dtos = newDtos;
            QStringList list;
            for (auto& dto : dtos) {
                list.push_back(QString::fromStdString(makeText(dto)));
            }
            model.setStringList(list);
        })
        .onError([this](const std::exception_ptr& exceptionPtr){
            try {
                std::rethrow_exception(exceptionPtr);
            } catch (BackendException& exception) {
                QMessageBox::warning(nullptr, "Error", exception.what());
            }
        }).execute();
}

std::string AdminMainWindowController::makeText(const UserDto& dto) {
    std::ostringstream output;
    output << dto.id << " - " << dto.email << " (" << dto.name << ")";
    return output.str();
}

void AdminMainWindowController::radioChanged(bool toggled) {
    if (!toggled)
        return;
    if (!saveAvailable)
        return;
    std::vector<UserRole> roles;
    if (ui->generalistRadioButton->isChecked())
        roles.push_back(UserRole::Generalist);
    if (ui->rezidentRadioButton->isChecked())
        roles.push_back(UserRole::Rezident);
    if (ui->specialistRadioButton->isChecked())
        roles.push_back(UserRole::Specialist);
    if (ui->primarRadioButton->isChecked())
        roles.push_back(UserRole::Primar);
    if (ui->adminCheckBox->isChecked())
        roles.push_back(UserRole::Admin);
    auto id = dtos.at(ui->listView->selectionModel()->selectedIndexes()[0].row()).id;
    HttpClientBuilder().withAuthentication()
                       .withType(HttpRequestType::POST)
                       .withUrl("/admin/user/{id}")
                       .withUrlParameter("id", std::to_string(id))
                       .withBody(roles)
                       .onSuccess<void>([this](){
                           populate();
                       })
                       .onError([this](std::exception_ptr exceptionPtr){
                           populate();
                           try {
                               std::rethrow_exception(std::move(exceptionPtr));
                           } catch (BackendException& exception) {
                               QMessageBox::warning(nullptr, "Error", exception.what());
                           }
                       })
                       .execute();
}

void AdminMainWindowController::selectionChanged(const QItemSelection& current, const QItemSelection&) {
    ui->adminCheckBox->setEnabled(false);
    ui->generalistRadioButton->setEnabled(false);
    ui->rezidentRadioButton->setEnabled(false);
    ui->specialistRadioButton->setEnabled(false);
    ui->primarRadioButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    saveAvailable = false;
    ui->adminCheckBox->setCheckState(Qt::Unchecked);
    ui->unassignedRadioButton->setChecked(true);
    ui->label->setText("Select an user");
    if (current.indexes().isEmpty()) {
        saveAvailable = true;
        return;
    }
    auto dto = dtos.at(current.indexes()[0].row());
    if (dto.id != TokenStorage::instance().getToken().userDetails.id)
        ui->adminCheckBox->setEnabled(true);
    ui->generalistRadioButton->setEnabled(true);
    ui->rezidentRadioButton->setEnabled(true);
    ui->specialistRadioButton->setEnabled(true);
    ui->primarRadioButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->label->setText(QString::fromStdString(makeText(dto)));
    for (auto& role : dto.roles) {
        switch (role) {
            case UserRole::User:
                break;
            case UserRole::Admin:
                ui->adminCheckBox->setCheckState(Qt::Checked);
                break;
            case UserRole::Generalist:
                ui->generalistRadioButton->setChecked(true);
                break;
            case UserRole::Rezident:
                ui->rezidentRadioButton->setChecked(true);
                break;
            case UserRole::Specialist:
                ui->specialistRadioButton->setChecked(true);
                break;
            case UserRole::Primar:
                ui->primarRadioButton->setChecked(true);
                break;
            case UserRole::Unassigned:
                ui->unassignedRadioButton->setChecked(true);
                break;
        }
    }
    saveAvailable = true;
}

void AdminMainWindowController::checkBoxChanged(int) {
    radioChanged(true);
}

void AdminMainWindowController::deletePressed() {
    auto id = dtos.at(ui->listView->selectionModel()->selectedIndexes()[0].row()).id;
    HttpClientBuilder().withAuthentication()
            .withType(HttpRequestType::GET)
            .withUrl("/admin/user/{id}/delete")
            .withUrlParameter("id", std::to_string(id))
            .onSuccess<void>([this](){
                populate();
            })
            .onError([this](std::exception_ptr exceptionPtr){
                populate();
                try {
                    std::rethrow_exception(std::move(exceptionPtr));
                } catch (BackendException& exception) {
                    QMessageBox::warning(nullptr, "Error", exception.what());
                }
            })
            .execute();
}

