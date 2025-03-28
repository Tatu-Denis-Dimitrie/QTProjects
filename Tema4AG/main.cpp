#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Grafic cu Algoritmi MST");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
