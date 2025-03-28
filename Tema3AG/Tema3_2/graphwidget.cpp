#include "GraphWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <queue>
#include <cstdlib>

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent) {
    setFixedSize(800, 600);
}

void GraphWidget::findConnectedComponents() {
    component.assign(nodes.size(), -1);
    colors.clear();

    int currentComponent = 0;
    for (size_t i = 0; i < nodes.size(); ++i    ) {
        if (component[i] == -1) {
            QColor randomColor(rand() % 256, rand() % 256, rand() % 256);
            colors.push_back(randomColor);
            bfs(i, currentComponent);
            currentComponent++;
        }
    }
}


void GraphWidget::bfs(int startNode, int compIndex) {
    std::queue<int> q;
    q.push(startNode);
    component[startNode] = compIndex;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : adjacencyList[node]) {
            if (component[neighbor] == -1) {
                component[neighbor] = compIndex;
                q.push(neighbor);
            }
        }
    }
}

bool GraphWidget::isNearNode(const QPoint& clickPos, const QPoint& nodePos) {
    return std::abs(clickPos.x() - nodePos.x()) <= NODE_RADIUS &&
           std::abs(clickPos.y() - nodePos.y()) <= NODE_RADIUS;
}

int GraphWidget::getNodeAtPosition(const QPoint& pos) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (isNearNode(pos, nodes[i])) {
            return i;
        }
    }
    return -1;
}

void GraphWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    for (const auto& pair : adjacencyList) {
        int nodeIndex = pair.first;

        for (int neighborIndex : pair.second) {
            if (nodeIndex < component.size() && neighborIndex < component.size()) {
                if (component[nodeIndex] == component[neighborIndex]) {
                    painter.setPen(colors[component[nodeIndex]]);
                } else {
                    painter.setPen(Qt::black);
                }
            } else {
                painter.setPen(Qt::black);
            }
            painter.drawLine(nodes[nodeIndex], nodes[neighborIndex]);
        }
    }
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i < component.size()) {
            if (i == selectedNode) {
                painter.setBrush(Qt::red);
            } else {
                painter.setBrush(colors[component[i]]);
            }
        } else {
            painter.setBrush(Qt::black);
        }
        painter.drawEllipse(nodes[i], NODE_RADIUS, NODE_RADIUS);
    }
}



void GraphWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        int nodeIndex = getNodeAtPosition(event->pos());
        if (nodeIndex == -1) {
            nodes.push_back(event->pos());
            adjacencyList[nodes.size() - 1] = {};
        }
    } else if (event->button() == Qt::RightButton) {
        int nodeIndex = getNodeAtPosition(event->pos());
        if (nodeIndex != -1) {
            if (selectedNode == -1) {
                selectedNode = nodeIndex;
            } else if (selectedNode != nodeIndex) {
                adjacencyList[selectedNode].push_back(nodeIndex);
                adjacencyList[nodeIndex].push_back(selectedNode);
                selectedNode = -1;
            } else {
                selectedNode = -1;
            }
        }
    }

    update();
}

void GraphWidget::colorGraph() {
    findConnectedComponents();
    update();
}
