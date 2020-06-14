//
// Created by laurentiu on 22.05.2020.
//

#ifndef RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
#define RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP

#include <QMainWindow>
#include <memory>
#include "../ui/ui_main.h"

#ifndef EDITMODE
constexpr auto hideImageLoader = true;
#else
constexpr auto hideImageLoader = false;
#endif

class MainWindowController : public QMainWindow
{
Q_OBJECT
public:
    MainWindowController();
    ~MainWindowController() override = default;
private:
    std::unique_ptr<Ui::Main> ui;
    QGraphicsScene scene;

private slots:
    void openImageLoaderButtonPushed();
    void searchButtonPushed();
    void openButtonPushed();
    void pathTextChanged(const QString &text);

};


#endif //RAULTESTQT_MAIN_WINDOW_CONTROLLER_HPP
