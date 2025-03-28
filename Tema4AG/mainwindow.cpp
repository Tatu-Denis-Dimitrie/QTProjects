#include "MainWindow.h"
#include "GraphWidget.h"
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    graphWidget = new GraphWidget(this);

    QRadioButton *directedButton = new QRadioButton("Graf orientat", this);
    QRadioButton *undirectedButton = new QRadioButton("Graf neorientat", this);

    directedButton->setChecked(true);

    QPushButton *primButton = new QPushButton("Arbore minim (Prim)", this);
    QPushButton *kruskalButton = new QPushButton("Arbore minim (Kruskal)", this);
    QPushButton *resetButton = new QPushButton("Resetare culori muchii", this);

    connect(directedButton, &QRadioButton::clicked, [this]() { graphWidget->setGraphOrientation(true); });
    connect(undirectedButton, &QRadioButton::clicked, [this]() { graphWidget->setGraphOrientation(false); });
    connect(primButton, &QPushButton::clicked, [this]() {
        QVector<Edge> mst = graphWidget->primMST();
        graphWidget->highlightMST(mst);
    });
    connect(kruskalButton, &QPushButton::clicked, [this]() {
        QVector<Edge> mst = graphWidget->kruskalMST();
        graphWidget->highlightMST(mst);
    });

    // Conectăm butonul de resetare a culorilor muchiilor
    connect(resetButton, &QPushButton::clicked, graphWidget, &GraphWidget::resetEdgeColors);

    layout->addWidget(graphWidget);
    layout->addWidget(directedButton);
    layout->addWidget(undirectedButton);
    layout->addWidget(primButton);
    layout->addWidget(kruskalButton);
    layout->addWidget(resetButton); // Adaugă butonul la layout
    setLayout(layout);
}
