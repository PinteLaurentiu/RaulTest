#include <QApplication>
#include "controller/login_controller.hpp"
#include "controller/main_window_controller.hpp"
#include "model/token.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    (new LoginController)->show();

    return QApplication::exec();
}
