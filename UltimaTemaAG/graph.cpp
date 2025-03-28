#include "graph.h"
#include "edge.h"
#include "mainwindow.h"
#include <fstream>
#include <queue>
#include <climits>

Graph::~Graph() {
}

void Graph::addNode(const QPoint& pos) {
    nodes.push_back(new Node(nodes.size(), pos));
}

Edge* Graph::addEdge(Node* first, Node* second, bool isorientation, int cost) {
    Edge* existingEdge = findEdge(first, second);
    if (existingEdge)
    {
        return existingEdge;
    }

    Edge* newEdge = new Edge(first, second, cost);
    edges.push_back(newEdge);
    return newEdge;
}

Edge* Graph::findEdge(Node* first, Node* second) {
    for (Edge* edge : edges) {
        if (edge->getFirst() == first && edge->getSecond() == second) {
            return edge;
        }
    }
    return nullptr;
}

const std::vector<Edge*>& Graph::getEdges() const { return edges; }
std::vector<Edge*>& Graph::getEdges() {
    return edges;
}

Node* Graph::getValueOfTheNode(int value) {
    for (Node* node : nodes) {
        if (node->getValue() == value){
            return node;
        }
    }
    return nullptr;
}

bool Graph::breadthFirstSearch(const std::vector<std::vector<int>>& remainingGraph, int start, int sink, std::vector<int>& parent) {
    std::vector<bool> visited(remainingGraph.size(), false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    parent[start] = -1;
    return performbreadthFirstSearch(remainingGraph, sink, visited, parent, q);
}


bool Graph::performbreadthFirstSearch(const std::vector<std::vector<int>>& remainingGraph, int sink, std::vector<bool>& visited, std::vector<int>& parent, std::queue<int>& que) {
    while (!que.empty()) {
        int u = que.front();
        que.pop();

        for (int v = 0; v < remainingGraph[u].size(); v++) {
            if (!visited[v] && remainingGraph[u][v] > 0) {
                que.push(v);
                parent[v] = u;
                visited[v] = true;

                if (v == sink)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int Graph::fordFulkerson(int start, int sink, MainWindow* mainWindow, std::vector<std::vector<std::vector<int>>>& remainingGraphs) {
    std::vector<std::vector<int>> remainingGraph = adjMat;
    std::vector<int> parent(nodes.size());
    int maxFlow = 0;

    while (breadthFirstSearch(remainingGraph, start, sink, parent)) {
        int pathFlow = findPathFlow(remainingGraph, start, sink, parent);
        updateremainingGraph(remainingGraph, start, sink, parent, pathFlow);
        remainingGraphs.push_back(remainingGraph);
        maxFlow += pathFlow;
    }

    return maxFlow;
}

int Graph::findPathFlow(const std::vector<std::vector<int>>& remainingGraph, int start, int sink, const std::vector<int>& parent) {
    int pathFlow = INT_MAX;

    for (int v = sink; v != start; v = parent[v]) {
        int u = parent[v];
        pathFlow = std::min(pathFlow, remainingGraph[u][v]);
    }

    return pathFlow;
}

void Graph::updateremainingGraph(std::vector<std::vector<int>>& remainingGraph, int start, int sink, const std::vector<int>& parent, int pathFlow) {
    for (int v = sink; v != start; v = parent[v]) {
        int u = parent[v];
        remainingGraph[u][v] -= pathFlow;
        remainingGraph[v][u] += pathFlow;
    }
}

void Graph::saveadjMat() {
    int size = nodes.size();
    adjMat.assign(size, std::vector<int>(size, 0));

    for (Edge* edge : edges) {
        int firstIndex = edge->getFirst()->getValue();
        int secondIndex = edge->getSecond()->getValue();
        adjMat[firstIndex][secondIndex] = edge->getCost();

        if (!orientation) adjMat[secondIndex][firstIndex] = edge->getCost();
    }
}

const std::vector<std::vector<int>>& Graph::getadjMat() const {
    return adjMat;
}

std::vector<bool> Graph::findNodesInRemainingGraph(const std::vector<std::vector<int>>& remainingGraph, int start) {
    std::vector<bool> visited(remainingGraph.size(), false);
    std::queue<int> queue;

    queue.push(start);
    visited[start] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int v = 0; v < remainingGraph[current].size(); ++v) {
            if (remainingGraph[current][v] > 0 && !visited[v]) {
                visited[v] = true;
                queue.push(v);
            }
        }
    }

    return visited;
}

