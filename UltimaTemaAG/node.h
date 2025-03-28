#ifndef NODE_H
#define NODE_H

#include <QPoint>

class Node {
public:
    Node();
    Node(int value, const QPoint& pos);

    int getValue() const;
    QPoint getPos() const;
    void setPos(const QPoint& pos);

private:
    int valueNode;
    QPoint posNode;
};

#endif // NODE_H
