#ifndef NODE_H
#define NODE_H

#include <QPoint>

class Node {
public:
    Node(QPoint pos, int index);
    ~Node();

    QPoint getPos() const;
    int getIndex() const;
    void setPos(const QPoint &newPos);
private:
    QPoint m_pos;
    int m_index;
};

#endif // NODE_H
