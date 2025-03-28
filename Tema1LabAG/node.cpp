#include "node.h"

Node::Node(QPoint pos, int index) : m_pos(pos), m_index(index) {}

Node::~Node() {}

QPoint Node::getPos() const {
    return m_pos;
}

int Node::getIndex() const {
    return m_index;
}

void Node::setPos(const QPoint &newPos) {
    m_pos = newPos;
}
