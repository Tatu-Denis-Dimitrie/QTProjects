#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPoint>
#include <vector>
#include <QKeyEvent>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {

private:
    Ui::MainWindow *ui;
    QGraphicsScene *window;
    QGraphicsView *view;
    std::vector<std::vector<int>> maze;
    std::vector<QPoint> exitPoints;
    QPoint startPosition;
    std::vector<std::vector<bool>> visitedCells;
    std::vector<std::vector<QPoint>> allPaths;
    int currentStep;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupGraphicsView();
    void loadMaze(const QString &filename);
    void renderMaze();
    void findPathsFromExits();
    void renderPath(const std::vector<QPoint> &path);
    std::vector<QPoint> getNeighbors(QPoint point);
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
