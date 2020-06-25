//
// Created by laurentiu on 25.06.2020.
//

#ifndef RAULTESTQT_ADMIN_MAIN_WINDOW_CONTROLLER_HPP
#define RAULTESTQT_ADMIN_MAIN_WINDOW_CONTROLLER_HPP


#include <QtWidgets/QMainWindow>
#include <memory>
#include <../ui/ui_admin_main.h>
#include <QtCore/QStringListModel>
#include <dto/user_dto.hpp>
#include <vector>
#include <QCloseEvent>

class AdminMainWindowController : public QMainWindow {
Q_OBJECT
public:
    AdminMainWindowController();
    ~AdminMainWindowController() override = default;

private:
    std::unique_ptr<Ui::AdminMainWindow> ui;
    QStringListModel model;

private slots:
    void radioChanged(bool toggled);
    void checkBoxChanged(int);
    void selectionChanged(const QItemSelection& current, const QItemSelection&);
    void deletePressed();

private:
    void populate();
    static std::string makeText(const UserDto& dto);

private:
    std::vector<UserDto> dtos;
    bool saveAvailable;
};


#endif //RAULTESTQT_ADMIN_MAIN_WINDOW_CONTROLLER_HPP
