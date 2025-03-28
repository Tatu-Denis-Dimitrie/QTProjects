#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
    Edge(int first, int second);
    ~Edge();

    int getFirst() const;
    int getSecond() const;
    void setFirst(int newFirst);
    void setSecond(int newSecond);

private:
    int m_first;
    int m_second;
};

#endif // EDGE_H
