#include "GameWindow.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameWindow game;
    game.show();

    return app.exec();
}
