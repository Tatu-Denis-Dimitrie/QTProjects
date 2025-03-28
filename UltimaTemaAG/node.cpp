#include "node.h"

Node::Node() {}

Node::Node(int value, const QPoint& pos)
    : valueNode(value), posNode(pos) {}

int Node::getValue() const {
    return valueNode;
}

QPoint Node::getPos() const {
    return posNode;
}

void Node::setPos(const QPoint& pos) {
    posNode = pos;
}
