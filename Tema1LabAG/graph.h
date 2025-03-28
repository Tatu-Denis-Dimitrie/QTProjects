#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include "node.h"
#include "edge.h"

class Graph {
public:
    Graph();
    ~Graph();

    void addNode(QPoint pos);
    void addEdge(int first, int second);
    void setGraphOrientation(bool oriented);
    bool isDuplicateEdge(const Edge& newEdge);
    void saveAdjacencyMatrix();
    int getNodeAtPosition(const QPoint &pos) const;
    QVector<Node>& getNodes();
    QVector<Edge>& getEdges();
    bool isOriented() const;

private:
    QVector<Node> nodes;
    QVector<Edge> edges;
    bool oriented;
};

#endif // GRAPH_H
