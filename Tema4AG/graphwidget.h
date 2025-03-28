#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QPainter>
#include "node.h"
#include "edge.h"


class GraphWidget : public QWidget {
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);

    void setGraphOrientation(bool directed);
    QVector<Edge> primMST();
    QVector<Edge> kruskalMST();
    void highlightMST(const QVector<Edge> &mstEdges);
    void resetEdgeColors();
    bool isGraphConnected();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<Node> nodes;
    QVector<Edge> edges;
    bool isDirected;
    int selectedNode, draggedNode;

    QVector<Edge> highlightedEdges;
    int getNodeAtPosition(int x, int y);
    void addEdge(int srcIndex, int destIndex);
    bool isDuplicateEdge(const Edge &newEdge);
    void saveAdjacencyMatrix();
    void drawArrow(QPainter &painter, const Node &src, const Node &dest);
};

#endif
