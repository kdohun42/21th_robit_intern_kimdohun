#include "../include/hhw2_pkg/main_window.hpp"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QString>
#include <QIcon>
#include <QDebug>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowDesign)
{
    ui->setupUi(this);


    QIcon icon("://ros-icon.png");

    this->setWindowIcon(icon);




    qnode = new QNode();


    QObject::connect(
        qnode,
        SIGNAL(rosShutDown()),
        this,
        SLOT(close())
        );




    // 기본 굵기 = 얇음
    selectedWidth = 1;


    // 기본 색상 = 빨강
    selectedR = 255;
    selectedG = 0;
    selectedB = 0;
}



void MainWindow::closeEvent(QCloseEvent* event)
{
    QMainWindow::closeEvent(event);
}




MainWindow::~MainWindow()
{
    delete qnode;

    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    selectedWidth = 1;

    qDebug() << "굵기 : 얇음";
}



void MainWindow::on_pushButton_2_clicked()
{
    selectedWidth = 4;

    qDebug() << "굵기 : 중간";
}




void MainWindow::on_pushButton_3_clicked()
{
    selectedWidth = 8;

    qDebug() << "굵기 : 두꺼움";
}




void MainWindow::on_pushButton_4_clicked()
{
    selectedR = 255;
    selectedG = 0;
    selectedB = 0;

    qDebug() << "색상 : RED";
}


void MainWindow::on_pushButton_5_clicked()
{
    selectedR = 0;
    selectedG = 255;
    selectedB = 0;

    qDebug() << "색상 : GREEN";
}



void MainWindow::on_pushButton_6_clicked()
{
    selectedR = 0;
    selectedG = 0;
    selectedB = 255;

    qDebug() << "색상 : BLUE";
}



void MainWindow::on_pushButton_7_clicked()
{

    QString input =
        ui->lineEdit->text()
            .trimmed()
            .toUpper();


    qDebug() << "입력 :" << input;


    if (
        input != "W" &&
        input != "A" &&
        input != "S" &&
        input != "D"
        )
    {
        QMessageBox::warning(
            this,
            "입력 오류",
            "W, A, S, D 중 하나를 입력하세요.\n\n"
            "W : 사각형\n"
            "A : 삼각형\n"
            "S : 원\n"
            "D : 오각형"
            );

        return;
    }


    qnode->setPen(
        selectedR,
        selectedG,
        selectedB,
        selectedWidth
        );


    if (input == "W")
    {
        qDebug() << "사각형 실행";

        qnode->drawSquare();
    }


    else if (input == "A")
    {
        qDebug() << "삼각형 실행";

        qnode->drawTriangle();
    }



    else if (input == "S")
    {
        qDebug() << "원 실행";

        qnode->drawCircle();
    }


    else if (input == "D")
    {
        qDebug() << "오각형 실행";

        qnode->drawPentagon();
    }
}
