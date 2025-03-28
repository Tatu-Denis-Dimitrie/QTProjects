#include "Graph.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <cmath>

Graph::Graph() : oriented(true) {}

Graph::~Graph() {}

QVector<Node>& Graph::getNodes() {
    return nodes;
}

QVector<Edge>& Graph::getEdges() {
    return edges;
}

bool Graph::isOriented() const {
    return oriented;
}

void Graph::addNode(QPoint pos) {
    nodes.append(Node(pos, nodes.size()));
    saveAdjacencyMatrix();
}

void Graph::addEdge(int first, int second) {
    Edge newEdge(first, second);
    if (!isDuplicateEdge(newEdge)) {
        edges.append(newEdge);
        if (!oriented) {
            edges.append(Edge(second, first));
        }
        saveAdjacencyMatrix();
    } else {
        QMessageBox::warning(nullptr, "Eroare", "Exista deja acest arc.");
    }
}

void Graph::setGraphOrientation(bool oriented) {
    this->oriented = oriented;
    saveAdjacencyMatrix();
}

bool Graph::isDuplicateEdge(const Edge& newEdge) {
    for (const auto& edge : edges) {
        if (edge.getFirst() == newEdge.getFirst() && edge.getSecond() == newEdge.getSecond()) {
            return true;
        }
    }
    return false;
}

void Graph::saveAdjacencyMatrix() {
    int numNodes = nodes.size();
    QVector<QVector<int>> adjacencyMatrix(numNodes, QVector<int>(numNodes, 0));

    for (const auto& edge : edges) {
        adjacencyMatrix[edge.getFirst()][edge.getSecond()] = 1;
        if (!oriented) {
            adjacencyMatrix[edge.getSecond()][edge.getFirst()] = 1;
        }
    }

    QFile file("matrice_adiacenta.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << numNodes << "\n";
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                out << adjacencyMatrix[i][j] << " ";
            }
            out << "\n";
        }
        file.close();
    }
}

int Graph::getNodeAtPosition(const QPoint &pos) const {
    for (int i = 0; i < nodes.size(); ++i) {
        int dx = nodes[i].getPos().x() - pos.x();
        int dy = nodes[i].getPos().y() - pos.y();
        if (std::sqrt(dx * dx + dy * dy) <= 10) {
            return i;
        }
    }
    return -1;
}
