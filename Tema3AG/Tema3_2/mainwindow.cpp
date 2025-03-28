#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    graphWidget = new GraphWidget(this);
    colorButton = new QPushButton("Colorează graful", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(graphWidget);
    layout->addWidget(colorButton);

    connect(colorButton, &QPushButton::clicked, graphWidget, &GraphWidget::colorGraph);
}
