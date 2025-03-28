#ifndef EDGE_H
#define EDGE_H

struct Edge {

    int srcIndex, destIndex, cost;

    Edge(int src, int dest, int cost) :
        srcIndex(src),
        destIndex(dest),
        cost(cost)
    {}

};
#endif
