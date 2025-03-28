#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), selectedNode(-1), draggedNode(-1)  {
    ui->setupUi(this);

    setMinimumSize(800, 600);
    ui->orientedButton->setChecked(true);

    connect(ui->orientedButton, &QRadioButton::clicked, this, [this]() { graph.setGraphOrientation(true); update(); });
    connect(ui->unorientedButton, &QRadioButton::clicked, this, [this]() { graph.setGraphOrientation(false); update(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter paint(this);
    paint.setPen(QPen(Qt::white, 1));

    for (const auto& edge : graph.getEdges()) {
        const Node& first = graph.getNodes()[edge.getFirst()];
        const Node& second = graph.getNodes()[edge.getSecond()];
        paint.drawLine(first.getPos().x(), first.getPos().y(), second.getPos().x(), second.getPos().y());
        if (graph.isOriented()) {
            drawArrow(paint, first, second);
        }
    }

    for (const auto& node : graph.getNodes()) {
        paint.setBrush(Qt::black);
        paint.setPen(Qt::white);
        paint.drawEllipse(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
        QRect textRect(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
        paint.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number(node.getIndex() + 1));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        graph.addNode(event->pos());
        update();
    } else if (event->button() == Qt::LeftButton) {
        int clickedNode = graph.getNodeAtPosition(event->pos());
        if (clickedNode != -1) {
            draggedNode = clickedNode;
            if (event->modifiers() & Qt::ShiftModifier) {
                if (selectedNode == -1) {
                    selectedNode = clickedNode;
                } else if (selectedNode != clickedNode) {
                    graph.addEdge(selectedNode, clickedNode);
                    selectedNode = -1;
                }
            }
        } else {
            draggedNode = -1;
        }
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (draggedNode != -1) {
        graph.getNodes()[draggedNode].setPos(event->pos());
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    draggedNode = -1;
    update();
}

void MainWindow::drawArrow(QPainter &paint, const Node &first, const Node &second) {
    double dx = second.getPos().x() - first.getPos().x();
    double dy = second.getPos().y() - first.getPos().y();
    double length = std::sqrt(dx * dx + dy * dy);
    double unitDx = dx / length;
    double unitDy = dy / length;

    int arrowLength = 20;
    int arrowWidth = 5;

    QPoint arrowP1(second.getPos().x() - arrowLength * unitDx + arrowWidth * unitDy,
                   second.getPos().y() - arrowLength * unitDy - arrowWidth * unitDx);
    QPoint arrowP2(second.getPos().x() - arrowLength * unitDx - arrowWidth * unitDy,
                   second.getPos().y() - arrowLength * unitDy + arrowWidth * unitDx);

    QPoint arrowHead[3] = { QPoint(second.getPos().x(), second.getPos().y()), arrowP1, arrowP2 };
    paint.setBrush(Qt::white);
    paint.setPen(Qt::white);
    paint.drawPolygon(arrowHead, 3);
}
