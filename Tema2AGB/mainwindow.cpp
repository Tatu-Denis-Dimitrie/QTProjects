#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), window(new QGraphicsScene(this)) {
    ui->setupUi(this);
    view = nullptr;
    currentStep = 0;
}

void MainWindow::setupGraphicsView() {
    view = new QGraphicsView(window, this);

    int windowWidth = this->width();
    int windowHeight = this->height();
    int viewWidth = windowWidth * 0.8;
    int viewHeight = windowHeight * 0.8;
    int x = (windowWidth - viewWidth) / 2;
    int y = (windowHeight - viewHeight) / 2;
    view->setGeometry(x, y, viewWidth, viewHeight);
    view->show();
}

MainWindow::~MainWindow() {
    delete ui;
    delete view;
}

void MainWindow::loadMaze(const QString &filename) {
    std::ifstream f(filename.toStdString());
    if (!f.is_open()) {
        qDebug() << "Error opening file!";
        return;
    }

    std::string line;
    int rowIndex = 0;
    while (std::getline(f, line)) {
        std::vector<int> rowData;
        for (int colIndex = 0; colIndex < line.size(); colIndex++) {
            if (line[colIndex] != ' ') {
                int value = line[colIndex] - '0';
                rowData.push_back(value);
                if (value == 3) {
                    startPosition = QPoint(rowIndex, colIndex);
                }
                if (value == 2) {
                    exitPoints.push_back(QPoint(rowIndex, colIndex));
                }
            }
        }
        maze.push_back(rowData);
        rowIndex++;
    }

    visitedCells = std::vector<std::vector<bool>>(maze.size(), std::vector<bool>(maze[0].size(), false));
    f.close();
}

void MainWindow::renderMaze() {
    int cellDimension = 70;

    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            QRect cellRect(col * cellDimension, row * cellDimension, cellDimension, cellDimension);
            QGraphicsRectItem *cellItem = window->addRect(cellRect);

            switch (maze[row][col]) {
            case 0:
                cellItem->setBrush(Qt::black);
                break;
            case 1:
                cellItem->setBrush(Qt::white);
                break;
            case 2:
                cellItem->setBrush(Qt::darkRed);
                break;
            case 3:
                cellItem->setBrush(Qt::darkBlue);
                break;
            default:
                break;
            }

            cellItem->setPen(QPen(Qt::darkGray));
        }
    }
}

std::vector<QPoint> MainWindow::getNeighbors(QPoint point) {
    std::vector<QPoint> neighborsList;
    std::vector<QPoint> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (const QPoint &direction : directions) {
        QPoint neighbor = point + direction;
        if (neighbor.x() >= 0 && neighbor.y() >= 0 &&
            neighbor.x() < maze.size() &&
            neighbor.y() < maze[0].size() &&
            maze[neighbor.x()][neighbor.y()] != 0) {
            neighborsList.push_back(neighbor);
        }
    }

    return neighborsList;
}

void MainWindow::findPathsFromExits() {
    for (const QPoint &exit : exitPoints) {
        std::queue<QPoint> queue;
        std::vector<QPoint> path;
        std::vector<std::vector<bool>> localVisited(maze.size(), std::vector<bool>(maze[0].size(), false));
        std::vector<std::vector<QPoint>> cameFrom(maze.size(), std::vector<QPoint>(maze[0].size()));

        queue.push(exit);
        localVisited[exit.x()][exit.y()] = true;

        bool pathFound = false;

        while (!queue.empty() && !pathFound) {
            QPoint current = queue.front();
            queue.pop();

            if (current == startPosition) {
                QPoint temp = current;
                path.push_back(temp);

                while (temp != exit) {
                    temp = cameFrom[temp.x()][temp.y()];
                    path.push_back(temp);
                }
                allPaths.push_back(path);
                pathFound = true;
            }

            for (const QPoint &neighbor : getNeighbors(current)) {
                if (!localVisited[neighbor.x()][neighbor.y()]) {
                    localVisited[neighbor.x()][neighbor.y()] = true;
                    cameFrom[neighbor.x()][neighbor.y()] = current;
                    queue.push(neighbor);
                }
            }
        }
    }
}

void MainWindow::renderPath(const std::vector<QPoint> &path) {
    int cellDimension = 70;

    if (currentStep < path.size()) {
        const QPoint &p = path[currentStep];
        QRect cellRect(p.y() * cellDimension, p.x() * cellDimension, cellDimension, cellDimension);
        QGraphicsRectItem *cellItem = window->addRect(cellRect);

        switch (maze[p.x()][p.y()]) {
        case 2:
            cellItem->setBrush(Qt::red);
            break;
        default:
            if (p == startPosition) {
                cellItem->setBrush(Qt::darkBlue);
            } else {
                cellItem->setBrush(Qt::green);
            }
            break;
        }

        cellItem->setPen(QPen(Qt::yellow));
        currentStep++;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        if (!allPaths.empty()) {
            if (currentStep < allPaths[0].size()) {
                renderPath(allPaths[0]);
            }
            else {
                currentStep = 0;
                allPaths.erase(allPaths.begin());
            }
        }
    }
}
