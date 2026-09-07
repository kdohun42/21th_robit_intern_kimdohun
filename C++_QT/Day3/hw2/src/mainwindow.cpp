#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dijkstraSolver(nullptr)
    , astarSolver(nullptr)
    , timer(new QTimer(this))
    , dijkstraRunning(false)
    , astarRunning(false)
{
    ui->setupUi(this);

    ui->widget->resizeMaze(10, 10);

    ui->widget_2->resizeMaze(10, 10);

    ui->label->setText(
        "최종 경로 비용 : "
        );

    ui->label_2->setText(
        "탐색한 노드 수 : "
        );

    ui->label_3->setText(
        "실행 시간 : "
        );

    ui->label_4->setText(
        "최종 경로 비용 : "
        );

    ui->label_5->setText(
        "탐색한 노드 수 : "
        );

    ui->label_6->setText(
        "실행 시간 : "
        );

    connect(
        ui->pushButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onObstacleButtonClicked
        );

    connect(
        ui->pushButton_2,
        &QPushButton::clicked,
        this,
        &MainWindow::onStartButtonClicked
        );

    connect(
        ui->pushButton_3,
        &QPushButton::clicked,
        this,
        &MainWindow::onEndButtonClicked
        );

    connect(
        ui->pushButton_4,
        &QPushButton::clicked,
        this,
        &MainWindow::onSizeButtonClicked
        );

    connect(
        ui->pushButton_5,
        &QPushButton::clicked,
        this,
        &MainWindow::onRunButtonClicked
        );

    connect(
        timer,
        &QTimer::timeout,
        this,
        &MainWindow::onTimerTick
        );

    connect(
        ui->widget,
        &MazeWidget::mazeEdited,
        this,
        &MainWindow::syncMaze
        );

    ui->widget_2->setMode(
        MazeWidget::Mode::None
        );
}

MainWindow::~MainWindow()
{
    delete dijkstraSolver;

    delete astarSolver;

    delete ui;
}

void MainWindow::syncMaze()
{
    ui->widget_2->setMapData(
        ui->widget->getMaze(),
        ui->widget->getStart(),
        ui->widget->getEnd()
        );
}

// 장애물

void MainWindow::onObstacleButtonClicked()
{
    ui->widget->setMode(
        MazeWidget::Mode::Obstacle
        );
}

// 시작점


void MainWindow::onStartButtonClicked()
{
    ui->widget->setMode(
        MazeWidget::Mode::Start
        );
}

// 도착점

void MainWindow::onEndButtonClicked()
{
    ui->widget->setMode(
        MazeWidget::Mode::End
        );
}

// 맵 크기

void MainWindow::onSizeButtonClicked()
{
    bool ok1;
    bool ok2;

    int rows =
        QInputDialog::getInt(
            this,
            "맵 크기 변경",
            "행(rows):",
            10,
            2,
            50,
            1,
            &ok1
            );

    if (!ok1)
    {
        return;
    }

    int cols =
        QInputDialog::getInt(
            this,
            "맵 크기 변경",
            "열(cols):",
            10,
            2,
            50,
            1,
            &ok2
            );

    if (!ok2)
    {
        return;
    }

    ui->widget->resizeMaze(
        rows,
        cols
        );

    ui->widget_2->resizeMaze(
        rows,
        cols
        );
}

// RUN

void MainWindow::onRunButtonClicked()
{
    timer->stop();

    delete dijkstraSolver;
    delete astarSolver;

    dijkstraSolver = nullptr;
    astarSolver = nullptr;

    auto maze =
        ui->widget->getMaze();

    auto start =
        ui->widget->getStart();

    auto end =
        ui->widget->getEnd();

    // Solver 생성

    dijkstraSolver =
        new DijkstraSolver(
            maze,
            start,
            end
            );

    astarSolver =
        new AStarSolver(
            maze,
            start,
            end
            );
    // 이전 시각화 삭제

    ui->widget->setVisited({});
    ui->widget->setPath({});

    ui->widget_2->setMapData(
        maze,
        start,
        end
        );

    ui->widget_2->setVisited({});
    ui->widget_2->setPath({});

    // Label 초기화

    ui->label->setText(
        "현재 비용 : 0"
        );

    ui->label_2->setText(
        "탐색한 노드 수 : 0"
        );

    ui->label_3->setText(
        "실행 시간 : 0.000000 ms"
        );

    ui->label_4->setText(
        "현재 비용 : 0"
        );

    ui->label_5->setText(
        "탐색한 노드 수 : 0"
        );

    ui->label_6->setText(
        "실행 시간 : 0.000000 ms"
        );

    // 실행 시작
    dijkstraRunning = true;

    astarRunning = true;

    timer->start(200);
}

// Timer

void MainWindow::onTimerTick()
{
    // Dijkstra

    if (
        dijkstraRunning &&
        dijkstraSolver != nullptr
        )
    {
        bool running =
            dijkstraSolver->step();

        // 시각화 갱신

        ui->widget->setVisited(
            dijkstraSolver->getVisited()
            );


        // 실시간 비용

        ui->label->setText(
            QString("현재 비용 : %1")
                .arg(
                    dijkstraSolver
                        ->getCurrentCost()
                    )
            );

        // 실시간 탐색 노드 수

        ui->label_2->setText(
            QString("탐색한 노드 수 : %1")
                .arg(
                    dijkstraSolver
                        ->getExploredCount()
                    )
            );

        // 실시간 실행 시간

        ui->label_3->setText(
            QString("실행 시간 : %1 ms")
                .arg(
                    dijkstraSolver
                        ->getElapsedTimeMs(),
                    0,
                    'f',
                    6
                    )
            );

        // 경로 발견

        if (
            dijkstraSolver->isPathFound()
            )
        {
            ui->widget->setPath(
                dijkstraSolver->getPath()
                );

            dijkstraRunning = false;

            // 최종 비용으로 변경
            ui->label->setText(
                QString("최종 경로 비용 : %1")
                    .arg(
                        dijkstraSolver
                            ->getCost()
                        )
                );
        }
        // 경로 없음

        else if (!running)
        {
            dijkstraRunning = false;

            ui->label->setText(
                "최종 경로 비용 : 경로 없음"
                );
        }
    }

    // A*

    if (
        astarRunning &&
        astarSolver != nullptr
        )
    {
        bool running =
            astarSolver->step();

        // 시각화

        ui->widget_2->setVisited(
            astarSolver->getVisited()
            );

        // 실시간 비용

        ui->label_4->setText(
            QString("현재 비용 : %1")
                .arg(
                    astarSolver
                        ->getCurrentCost()
                    )
            );

        // 실시간 탐색 노드

        ui->label_5->setText(
            QString("탐색한 노드 수 : %1")
                .arg(
                    astarSolver
                        ->getExploredCount()
                    )
            );

        // 실시간 실행 시간

        ui->label_6->setText(
            QString("실행 시간 : %1 ms")
                .arg(
                    astarSolver
                        ->getElapsedTimeMs(),
                    0,
                    'f',
                    6
                    )
            );

        // 경로 발견

        if (
            astarSolver->isPathFound()
            )
        {
            ui->widget_2->setPath(
                astarSolver->getPath()
                );

            astarRunning = false;

            ui->label_4->setText(
                QString("최종 경로 비용 : %1")
                    .arg(
                        astarSolver
                            ->getCost()
                        )
                );
        }

        // 경로 없음

        else if (!running)
        {
            astarRunning = false;

            ui->label_4->setText(
                "최종 경로 비용 : 경로 없음"
                );
        }
    }

    // 둘 다 종료

    if (
        !dijkstraRunning &&
        !astarRunning
        )
    {
        timer->stop();
    }
}
