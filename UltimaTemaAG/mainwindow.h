#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <QVBoxLayout>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initializeUI();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;

    void radioButtonToggled(bool checked);

    void calculateMaxFlow();
    void updateremainingGraph(const std::vector<std::vector<int>>& remainingGraph);
    void showremainingGraphs(const std::vector<std::vector<std::vector<int>>>& remainingGraphs, int maxFlow);

private:
    Ui::MainWindow *ui;
    Graph mainGraph;
    Node* m_selectedNode;
    bool firstNodeSelected;
    QPoint mousePosNode;

    std::vector<std::vector<std::vector<int>>> remainingGraphs;
    int currentRemainingIndex;
    int maxFlow;

    void handleLeftClick(QMouseEvent *m);
    void handleRightClick(QMouseEvent *m);
    bool checkNodeOverlap(QMouseEvent *m);
    bool isNodeClicked(Node* n, QMouseEvent *m);
    void createEdgeBetweenNodes(Node* n);
    void drawEdges(QPainter &painter);
    void drawEdge(QPainter &painter, Edge* edge, int nodeRadius);
    bool hasReverseEdgeBetweenNodes(Node* first, Node* second);
    void draworientationEdge(QPainter &painter, QPointF& adjustedP1, QPointF& adjustedP2, QPointF& offset,
                          double dx, double dy, double dist, Edge* edge, bool hasReverseEdge);
    void drawNodes(QPainter &painter);
    void drawNode(QPainter &painter, Node* node, int nodeRadius);
    void addEdgeInremainingGraph(const std::vector<std::vector<int>>& remainingGraph, const std::vector<bool>& reachable, int u, int v);
};

#endif // MAINWINDOW_H
