#include <QApplication>
#include "mainwindow.h"
#include "game.h"

Game *game;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/images/icon.png"));
    game = new Game();
    game->show();
    //comezamos el juego
    game->start();
    return a.exec();
}
