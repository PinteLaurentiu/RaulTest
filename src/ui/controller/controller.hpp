//
// Created by laurentiu on 22.05.2020.
//

#ifndef RAULTESTQT_CONTROLLER_HPP
#define RAULTESTQT_CONTROLLER_HPP

#include <QMainWindow>
#include <memory>
#include "../ui/ui_main.h"

class Controller : public QMainWindow
{
Q_OBJECT
public:
    Controller();
    ~Controller() override = default;
private:
    std::unique_ptr<Ui::Main> controller;
    QGraphicsScene scene;
};


#endif //RAULTESTQT_CONTROLLER_HPP
