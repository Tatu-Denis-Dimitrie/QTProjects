#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <vector>
#include <unordered_map>

class GraphWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<QPoint> nodes;
    std::unordered_map<int, std::vector<int>> adjacencyList;
    std::vector<int> component;
    std::vector<QColor> colors;
    int selectedNode = -1;
    const int NODE_RADIUS = 15;

    void findConnectedComponents();
    void bfs(int startNode, int compIndex);
    bool isNearNode(const QPoint& clickPos, const QPoint& nodePos);
    int getNodeAtPosition(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

public:
    explicit GraphWidget(QWidget* parent = nullptr);
    void colorGraph();
};

#endif
