#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <set>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_selectedNode(nullptr),
    firstNodeSelected(false),
    currentRemainingIndex(0), maxFlow(0)
{
    ui->setupUi(this);
    mainGraph.setorientation(true);
    initializeUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeUI() {
    QVBoxLayout* layout = new QVBoxLayout;
    QPushButton* generateMaxFlowButton = new QPushButton("Flow maxim", this);

    layout->addWidget(generateMaxFlowButton, 0, Qt::AlignTop);


    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(generateMaxFlowButton, &QPushButton::clicked, this, &MainWindow::calculateMaxFlow);
}

void MainWindow::radioButtonToggled(bool checked)
{
    mainGraph.setorientation(checked);
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    if (m->button() == Qt::LeftButton) {
        handleLeftClick(m);
    }
    if (m->button() == Qt::RightButton) {
        handleRightClick(m);
    }
}

void MainWindow::handleLeftClick(QMouseEvent *m) {
    bool overlap = checkNodeOverlap(m);
    if (!overlap) {
        mainGraph.addNode(m->pos());
        mainGraph.saveadjMat();
    }
    update();
}

bool MainWindow::checkNodeOverlap(QMouseEvent *m) {
    for (auto& n : mainGraph.getNodes()) {
        if (std::hypot(n->getPos().x() - m->pos().x(), n->getPos().y() - m->pos().y()) < 30) {
            return true;
        }
    }
    return false;
}

void MainWindow::handleRightClick(QMouseEvent *m) {
    std::vector<Node*> nodes = mainGraph.getNodes();
    for (auto& n : nodes) {
        if (isNodeClicked(n, m)) {
            createEdgeBetweenNodes(n);
            break;
        }
    }
}

bool MainWindow::isNodeClicked(Node* n, QMouseEvent *m) {
    return (std::pow(n->getPos().x() - m->pos().x(), 2) + std::pow(n->getPos().y() - m->pos().y(), 2) < 100);
}

void MainWindow::createEdgeBetweenNodes(Node* n) {
    if (firstNodeSelected) {
        if (m_selectedNode->getValue() != n->getValue()) {
            bool ok;
            int cost = QInputDialog::getInt(this, "Introduceti costul", "Costul muchiei:", 1, 1, 1000, 1, &ok);
            if (ok) {
                mainGraph.addEdge(m_selectedNode, n, mainGraph.isGraphorientation(), cost);
                mainGraph.saveadjMat();
                update();
            }
        } else {
            qDebug() << "Cannot create an edge to the same node.";
        }
        firstNodeSelected = false;
    } else {
        firstNodeSelected = true;
        m_selectedNode = n;
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawEdges(painter);
    drawNodes(painter);
}

void MainWindow::drawEdges(QPainter &painter) {
    const int nodeRadius = 10;
    for (Edge* edge : mainGraph.getEdges()) {
        drawEdge(painter, edge, nodeRadius);
    }
}

void MainWindow::drawEdge(QPainter &painter, Edge* edge, int nodeRadius) {
    Node* first = edge->getFirst();
    Node* second = edge->getSecond();

    QPointF p1 = first->getPos();
    QPointF p2 = second->getPos();
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double dist = std::sqrt(dx * dx + dy * dy);

    QPointF adjustedP1 = p1 + QPointF(dx / dist * nodeRadius, dy / dist * nodeRadius);
    QPointF adjustedP2 = p2 - QPointF(dx / dist * nodeRadius, dy / dist * nodeRadius);

    bool hasReverseEdge = hasReverseEdgeBetweenNodes(first, second);

    QPointF offset(0, 0);
    if (hasReverseEdge) {
        offset = QPointF(-dy / dist * 10, dx / dist * 10);
    }

    QPen pen;
    if (edge->isHighlighted()) {
        pen = QPen(Qt::red, 2);
    } else {
        pen = QPen(Qt::blue, 1);
    }
    painter.setPen(pen);

    if (mainGraph.isGraphorientation()) {
        draworientationEdge(painter, adjustedP1, adjustedP2, offset, dx, dy, dist, edge, hasReverseEdge);
    } else {
        painter.drawLine(adjustedP1, adjustedP2);
    }

    painter.drawText((adjustedP1 + adjustedP2) / 2 + offset, QString::number(edge->getCost()));
}

bool MainWindow::hasReverseEdgeBetweenNodes(Node* first, Node* second) {
    for (Edge* reverseEdge : mainGraph.getEdges()) {
        if (reverseEdge->getFirst() == second && reverseEdge->getSecond() == first) {
            return true;
        }
    }
    return false;
}

void MainWindow::draworientationEdge(QPainter &painter, QPointF& adjustedP1, QPointF& adjustedP2, QPointF& offset,
                                  double dx, double dy, double dist, Edge* edge, bool hasReverseEdge) {
    painter.drawLine(adjustedP1 + offset, adjustedP2 + offset);
    QPointF arrowP1 = adjustedP2 + offset - QPointF(dx / dist * 10 - dy / dist * 5, dy / dist * 10 + dx / dist * 5);
    QPointF arrowP2 = adjustedP2 + offset - QPointF(dx / dist * 10 + dy / dist * 5, dy / dist * 10 - dx / dist * 5);
    painter.drawLine(adjustedP2 + offset, arrowP1);
    painter.drawLine(adjustedP2 + offset, arrowP2);

    if (hasReverseEdge) {
        painter.drawLine(adjustedP2 - offset, adjustedP1 - offset);
        QPointF reverseArrowP1 = adjustedP1 - offset - QPointF(-dx / dist * 10 - dy / dist * 5, -dy / dist * 10 + dx / dist * 5);
        QPointF reverseArrowP2 = adjustedP1 - offset - QPointF(-dx / dist * 10 + dy / dist * 5, -dy / dist * 10 - dx / dist * 5);
        painter.drawLine(adjustedP1 - offset, reverseArrowP1);
        painter.drawLine(adjustedP1 - offset, reverseArrowP2);
    }
}

void MainWindow::drawNodes(QPainter &painter) {
    const int nodeRadius = 10;
    for (Node* node : mainGraph.getNodes()) {
        drawNode(painter, node, nodeRadius);
    }
}

void MainWindow::drawNode(QPainter &painter, Node* node, int nodeRadius) {
    QRectF rect(node->getPos().x() - nodeRadius, node->getPos().y() - nodeRadius, 2 * nodeRadius, 2 * nodeRadius);
    painter.setPen(Qt::darkCyan);
    painter.drawEllipse(rect);
    painter.drawText(rect, Qt::AlignCenter, QString::number(node->getValue() + 1));
}

void MainWindow::showremainingGraphs(const std::vector<std::vector<std::vector<int>>>& remainingGraphs, int maxFlow) {
    static int currentIteration = 0;
    if (currentIteration >= remainingGraphs.size()) {
        QMessageBox::information(this, "Flux Maxim", "Fluxul maxim este: " + QString::number(maxFlow));
        currentIteration = 0;
        return;
    }
    updateremainingGraph(remainingGraphs[currentIteration]);
    currentIteration++;
}

void MainWindow::calculateMaxFlow() {
    int start = 0;
    int sink = mainGraph.getNodes().size() - 1;

    remainingGraphs.clear();
    currentRemainingIndex = 0;

    maxFlow = mainGraph.fordFulkerson(start, sink, this, remainingGraphs);

    if (!remainingGraphs.empty()) {
        showremainingGraphs(remainingGraphs, maxFlow);
    } else {
        QMessageBox::information(this, "Flux Maxim", "Fluxul maxim este: " + QString::number(maxFlow));
    }
}

void MainWindow::updateremainingGraph(const std::vector<std::vector<int>>& remainingGraph) {
    std::vector<bool> reachable = mainGraph.findNodesInRemainingGraph(remainingGraph, 0);

    mainGraph.getEdges().clear();

    for (int u = 0; u < remainingGraph.size(); ++u) {
        for (int v = 0; v < remainingGraph[u].size(); ++v) {
            addEdgeInremainingGraph(remainingGraph, reachable, u, v);
        }
    }

    update();
}

void MainWindow::addEdgeInremainingGraph(const std::vector<std::vector<int>>& remainingGraph, const std::vector<bool>& reachable, int u, int v) {
    if (remainingGraph[u][v] > 0) {
        Node* nodeU = mainGraph.getValueOfTheNode(u);
        Node* nodeV = mainGraph.getValueOfTheNode(v);

        Edge* edge = mainGraph.addEdge(nodeU, nodeV, true, remainingGraph[u][v]);

        if ((reachable[u] && !reachable[v]) || (!reachable[u] && reachable[v])) {
            edge->setHighlight(true);
        }
    }

    if (remainingGraph[v][u] > 0) {
        Node* nodeV = mainGraph.getValueOfTheNode(v);
        Node* nodeU = mainGraph.getValueOfTheNode(u);

        Edge* reverseEdge = mainGraph.addEdge(nodeV, nodeU, true, remainingGraph[v][u]);

        if ((reachable[v] && !reachable[u]) || (!reachable[v] && reachable[u])) {
            reverseEdge->setHighlight(true);
        }
    }
}
