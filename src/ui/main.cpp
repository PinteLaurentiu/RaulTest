#include <QApplication>
#include <src/ui/controller/login_controller.hpp>
#include <src/ui/controller/open_windows_cache.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    auto controller = std::make_unique<LoginController>();
    controller->show();
    OpenWindowsCache::instance().save(std::move(controller));
    return QApplication::exec();
}
