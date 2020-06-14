#include "controller/image_loader_controller.hpp"
#include <QApplication>
#include <src/ui/controller/main_window_controller.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindowController controller;
    controller.show();

    return QApplication::exec();
}
