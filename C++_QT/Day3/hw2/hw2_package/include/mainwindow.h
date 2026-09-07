#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "dijkstrasolver.h"
#include "astarsolver.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void onTimerTick();

    void onObstacleButtonClicked();

    void onStartButtonClicked();

    void onEndButtonClicked();

    void onSizeButtonClicked();

    void onRunButtonClicked();

    void syncMaze();

private:
    Ui::MainWindow *ui;

    DijkstraSolver *dijkstraSolver;

    AStarSolver *astarSolver;

    QTimer *timer;

    bool dijkstraRunning;

    bool astarRunning;
};

#endif
