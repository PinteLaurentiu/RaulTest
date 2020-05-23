#include "controller/controller.hpp"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Controller controller;
    controller.show();

    return QApplication::exec();
}
