#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.setupGraphicsView();
    w.loadMaze("map.txt");
    w.renderMaze();
    w.findPathsFromExits();

    return a.exec();
}
