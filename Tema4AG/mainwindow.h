#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class GraphWidget;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    GraphWidget *graphWidget;
};

#endif
