#include "edge.h"

Edge::Edge() {}

Edge::Edge(Node* first, Node* second, int cost)
    : firstNode(first), secondNode(second), costNode(cost), highlightedNode(false) {}

Node* Edge::getFirst() const {
    return firstNode;
}

Node* Edge::getSecond() const {
    return secondNode;
}

int Edge::getCost() const {
    return costNode;
}

void Edge::setCost(int cost) {
    costNode = cost;
}

void Edge::setHighlight(bool highlight) {
    highlightedNode = highlight;
}

bool Edge::isHighlighted() const {
    return highlightedNode;
}
