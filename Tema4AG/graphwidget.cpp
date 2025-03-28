#include "GraphWidget.h"
#include <QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <cmath>
#include <algorithm>
#include <queue>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), selectedNode(-1), draggedNode(-1), isDirected(true) {}

void GraphWidget::setGraphOrientation(bool directed) {
    isDirected = directed;
    saveAdjacencyMatrix();
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1));

    for (const auto &edge : edges) {
        const Node &src = nodes[edge.srcIndex];
        const Node &dest = nodes[edge.destIndex];
        painter.drawLine(src.x, src.y, dest.x, dest.y);

        int midX = (src.x + dest.x) / 2;
        int midY = (src.y + dest.y) / 2;
        painter.setPen(Qt::blue);
        painter.drawText(midX, midY, QString::number(edge.cost));
        painter.setPen(Qt::black);

        if (isDirected) {
            drawArrow(painter, src, dest);
        }
    }

    painter.setPen(QPen(Qt::green, 3));
    for (const auto &edge : highlightedEdges) {
        const Node &src = nodes[edge.srcIndex];
        const Node &dest = nodes[edge.destIndex];
        painter.drawLine(src.x, src.y, dest.x, dest.y);
    }

    for (const auto &node : nodes) {
        painter.setBrush(Qt::black);
        painter.drawEllipse(node.x - 10, node.y - 10, 20, 20);
        painter.setPen(Qt::white);
        painter.drawText(node.x - 5, node.y + 5, QString::number(node.index + 1));
    }
}

void GraphWidget::resetEdgeColors() {
    highlightedEdges.clear();
    update();
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    const int minDistance = 30;

    if (event->button() == Qt::RightButton) {
        bool overlap = false;
        for (const Node &node : nodes) {
            int dx = node.x - x;
            int dy = node.y - y;
            if (sqrt(dx * dx + dy * dy) < minDistance) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            nodes.append(Node(x, y, nodes.size()));
            saveAdjacencyMatrix();
            update();
        } else {
            QMessageBox::warning(this, "Eroare", "Nodul este prea aproape de alt nod.");
        }
    } else if (event->button() == Qt::LeftButton) {
        int clickedNode = getNodeAtPosition(x, y);
        if (clickedNode != -1) {
            draggedNode = clickedNode;
            if (event->modifiers() & Qt::ShiftModifier) {
                if (selectedNode == -1) {
                    selectedNode = clickedNode;
                } else if (selectedNode != clickedNode) {
                    addEdge(selectedNode, clickedNode);
                    selectedNode = -1;
                }
            }
        } else {
            draggedNode = -1;
        }
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
    if (draggedNode != -1) {
        nodes[draggedNode].x = event->x();
        nodes[draggedNode].y = event->y();
        saveAdjacencyMatrix();
        update();
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event) {
    draggedNode = -1;
}

int GraphWidget::getNodeAtPosition(int x, int y) {
    for (int i = 0; i < nodes.size(); i++) {
        int dx = nodes[i].x - x;
        int dy = nodes[i].y - y;
        if (std::sqrt(dx * dx + dy * dy) <= 10) {
            return i;
        }
    }
    return -1;
}

void GraphWidget::addEdge(int srcIndex, int destIndex) {
    bool ok;
    int cost = QInputDialog::getInt(this, "Cost arc",
                                    QString("Introduceți costul arcului dintre %1 și %2:").arg(srcIndex + 1).arg(destIndex + 1),
                                    1, 1, 1000, 1, &ok);

    if (ok) {
        Edge newEdge(srcIndex, destIndex, cost);
        if (!isDuplicateEdge(newEdge)) {
            edges.append(newEdge);
            if (!isDirected) {
                edges.append(Edge(destIndex, srcIndex, cost));
            }
            saveAdjacencyMatrix();
            update();
        } else {
            QMessageBox::warning(this, "Multigraf", "Arcul există deja.");
        }
    }
}

bool GraphWidget::isDuplicateEdge(const Edge &newEdge) {
    for (const auto &edge : edges) {
        if (edge.srcIndex == newEdge.srcIndex && edge.destIndex == newEdge.destIndex) {
            return true;
        }
    }
    return false;
}

void GraphWidget::saveAdjacencyMatrix() {
    int numNodes = nodes.size();
    QVector<QVector<int>> adjacencyMatrix(numNodes, QVector<int>(numNodes, 0));

    for (const auto &edge : edges) {
        adjacencyMatrix[edge.srcIndex][edge.destIndex] = edge.cost;
        if (!isDirected) {
            adjacencyMatrix[edge.destIndex][edge.srcIndex] = edge.cost;
        }
    }

    QFile file("matrice_adiacenta.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << numNodes << "\n";
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                out << adjacencyMatrix[i][j] << " ";
            }
            out << "\n";
        }
        file.close();
    }
}

void GraphWidget::drawArrow(QPainter &painter, const Node &src, const Node &dest) {
    double dx = dest.x - src.x;
    double dy = dest.y - src.y;
    double length = std::sqrt(dx * dx + dy * dy);
    double unitDx = dx / length;
    double unitDy = dy / length;

    int arrowLength = 20;
    int arrowWidth = 5;

    QPoint arrowP1(dest.x - arrowLength * unitDx + arrowWidth * unitDy,
                   dest.y - arrowLength * unitDy - arrowWidth * unitDx);
    QPoint arrowP2(dest.x - arrowLength * unitDx - arrowWidth * unitDy,
                   dest.y - arrowLength * unitDy + arrowWidth * unitDx);

    QPoint arrowHead[3] = { QPoint(dest.x, dest.y), arrowP1, arrowP2 };
    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead, 3);
}

bool GraphWidget::isGraphConnected() {
    if (nodes.isEmpty()) return false;

    QVector<bool> visited(nodes.size(), false);
    std::queue<int> q;
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (const Edge &edge : edges) {
            if (edge.srcIndex == node && !visited[edge.destIndex]) {
                visited[edge.destIndex] = true;
                q.push(edge.destIndex);
            }
        }
    }

    for (bool v : visited) {
        if (!v) {
            return false;
        }
    }

    return true;
}

QVector<Edge> GraphWidget::primMST() {
    if (!isGraphConnected()) {
        QMessageBox::warning(nullptr, "Eroare", "Graful nu este conex. Algoritmul Prim nu poate rula.");
        return {};
    }

    QVector<bool> inMST(nodes.size(), false);
    QVector<Edge> mstEdges;
    int totalNodes = nodes.size();
    int totalCost = 0;

    auto cmp = [](const Edge &a, const Edge &b) { return a.cost > b.cost; };
    std::priority_queue<Edge, QVector<Edge>, decltype(cmp)> pq(cmp);

    inMST[0] = true;
    for (const Edge &e : edges) {
        if (e.srcIndex == 0) pq.push(e);
    }

    while (!pq.empty() && mstEdges.size() < totalNodes - 1) {
        Edge edge = pq.top();
        pq.pop();
        if (!inMST[edge.destIndex]) {
            mstEdges.append(edge);
            inMST[edge.destIndex] = true;
            totalCost += edge.cost;

            for (const Edge &e : edges) {
                if (e.srcIndex == edge.destIndex && !inMST[e.destIndex]) {
                    pq.push(e);
                }
            }
        }
    }
    QMessageBox::information(nullptr, "Suma Costurilor", QString("Suma costurilor muchiilor din MST este: %1").arg(totalCost));
    return mstEdges;
}

QVector<Edge> GraphWidget::kruskalMST() {
    if (!isGraphConnected()) {
        QMessageBox::warning(nullptr, "Eroare", "Graful nu este conex. Algoritmul Prim nu poate rula.");
        return {};
    }

    QVector<Edge> mstEdges;
    QVector<int> component(nodes.size());

    for (int i = 0; i < nodes.size(); ++i) {
        component[i] = i;
    }

    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    for (const Edge &edge : edges) {
        int srcComponent = component[edge.srcIndex];
        int destComponent = component[edge.destIndex];

        if (srcComponent != destComponent) {
            mstEdges.append(edge);

            for (int i = 0; i < component.size(); ++i) {
                if (component[i] == destComponent) {
                    component[i] = srcComponent;
                }
            }
        }

        if (mstEdges.size() == nodes.size() - 1) {
            break;
        }
    }

    return mstEdges;
}


void GraphWidget::highlightMST(const QVector<Edge> &mstEdges) {
    highlightedEdges = mstEdges;
    update();
}
