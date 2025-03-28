#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <stack>
#include <QGraphicsTextItem>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;
    std::stack<int> topoStack;
    bool hasCycle = false;

    void DFS(int startNode, std::vector<int>& state) {
        std::stack<int> stack;
        stack.push(startNode);

        while (!stack.empty()) {
            int node = stack.top();
            if (state[node] == 0) {
                state[node] = 1;
            }

            bool allNeighborsVisited = true;
            for (int neighbor : adj[node]) {
                if (state[neighbor] == 0) {
                    stack.push(neighbor);
                    allNeighborsVisited = false;
                    break;
                } else if (state[neighbor] == 1) {
                    hasCycle = true;
                }
            }

            if (allNeighborsVisited) {
                state[node] = 2;
                topoStack.push(node);
                stack.pop();
            }
        }
    }

public:
    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    bool topologicalSort(std::vector<int>& result) {
        std::vector<int> state(V, 0);
        for (int i = 0; i < V; i++) {
            if (state[i] == 0) {
                DFS(i, state);
            }
        }
        if (hasCycle) return false;
        while (!topoStack.empty()) {
            result.push_back(topoStack.top());
            topoStack.pop();
        }
        return true;
    }
};

void drawGraph(const std::vector<int>& sortedNodes) {
    QGraphicsScene* scene = new QGraphicsScene();
    int radius = 30;
    int spacing = 100;
    int x = 50;
    int y = 50;
    int totalWidth = (sortedNodes.size() - 1) * spacing;
    x = (800 - totalWidth) / 2;

    std::vector<QPointF> nodePositions;
    for (int i = 0; i < sortedNodes.size(); i++) {
        scene->addEllipse(x, y, radius, radius, QPen(), QBrush(Qt::green));
        QGraphicsTextItem* text = scene->addText(QString::number(sortedNodes[i]));
        text->setPos(x + radius / 4, y - radius );
        nodePositions.push_back(QPointF(x + radius / 2, y + radius / 2));

        x += spacing;
    }
    for (int i = 1; i < sortedNodes.size(); i++) {
        scene->addLine(nodePositions[i - 1].x(), nodePositions[i - 1].y(),
                       nodePositions[i].x(), nodePositions[i].y(), QPen(Qt::black));
    }

    QGraphicsView* view = new QGraphicsView(scene);
    view->setWindowTitle("Graf Sortat Topologic");
    view->resize(800, 200);
    view->show();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Graph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(5, 3);
    g.addEdge(5, 6);
    g.addEdge(6, 7);

    std::vector<int> sortedNodes;
    if (!g.topologicalSort(sortedNodes)) {
        QMessageBox::critical(nullptr, "Eroare", "Graful are circuite și nu poate fi sortat topologic.");
        return -1;
    }

    QFile file("sorted_graph.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        for (int node : sortedNodes) {
            out << node << " ";
        }
        file.close();
    }
    drawGraph(sortedNodes);

    return app.exec();
}
