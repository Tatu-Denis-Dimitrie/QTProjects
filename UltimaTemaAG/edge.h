#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge {
public:
    Edge();
    Edge(Node* first, Node* second, int cost = 1);

    Node* getFirst() const;
    Node* getSecond() const;
    int getCost() const;
    void setCost(int cost);
    void setHighlight(bool highlight);
    bool isHighlighted() const;

private:
    Node* firstNode;
    Node* secondNode;
    int costNode;
    bool highlightedNode;
};

#endif // EDGE_H
