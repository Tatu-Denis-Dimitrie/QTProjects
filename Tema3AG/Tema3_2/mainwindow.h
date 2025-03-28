#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "GraphWidget.h"

class MainWindow : public QWidget {
    Q_OBJECT

private:
    GraphWidget* graphWidget;
    QPushButton* colorButton;

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif
