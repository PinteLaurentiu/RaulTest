#include <QApplication>
#include <src/ui/controller/login_controller.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    (new LoginController)->show();
    return QApplication::exec();
}
