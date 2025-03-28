#include "edge.h"

Edge::Edge(int first, int second) : m_first(first), m_second(second) {}

Edge::~Edge() {}

int Edge::getFirst() const {
    return m_first;
}

int Edge::getSecond() const {
    return m_second;
}

void Edge::setFirst(int newFirst) {
    m_first = newFirst;
}

void Edge::setSecond(int newSecond) {
    m_second = newSecond;
}
