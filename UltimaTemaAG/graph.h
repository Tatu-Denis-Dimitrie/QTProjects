#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>
#include <queue>
#include "node.h"
#include "edge.h"

class MainWindow;

class Graph {
public:
    Graph() : orientation(false) {}
    ~Graph();

    void addNode(const QPoint& pos);
    Node* getValueOfTheNode(int value);
    const std::vector<Node*>& getNodes() const { return nodes; }

    Edge* addEdge(Node* first, Node* second, bool isorientation, int cost);
    const std::vector<Edge*>& getEdges() const;
    std::vector<Edge*>& getEdges();
    void setorientation(bool value) { orientation = value; }
    bool isGraphorientation() const { return orientation; }

    bool breadthFirstSearch(const std::vector<std::vector<int>>& remainingGraph, int start, int sink, std::vector<int>& parent);
    int fordFulkerson(int start, int sink, MainWindow* mainWindow, std::vector<std::vector<std::vector<int>>>& remainingGraphs);
    std::vector<bool> findNodesInRemainingGraph(const std::vector<std::vector<int>>& remainingGraph, int start);

    void saveadjMat();
    const std::vector<std::vector<int>>& getadjMat() const;

private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;

    std::vector<std::vector<int>> adjMat;

    bool orientation;
    Edge* findEdge(Node* first, Node* second);

    bool performbreadthFirstSearch(const std::vector<std::vector<int>>& remainingGraph, int sink, std::vector<bool>& visited, std::vector<int>& parent, std::queue<int>& q);

    int findPathFlow(const std::vector<std::vector<int>>& remainingGraph, int start, int sink, const std::vector<int>& parent);
    void updateremainingGraph(std::vector<std::vector<int>>& remainingGraph, int start, int sink, const std::vector<int>& parent, int pathFlow);

    void initializeReachableNodesSearch(int start, std::vector<bool>& visited, std::queue<int>& queue);
};

#endif // GRAPH_H
