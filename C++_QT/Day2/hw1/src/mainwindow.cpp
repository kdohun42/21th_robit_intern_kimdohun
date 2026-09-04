#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->slider, &QSlider::valueChanged, this, &MainWindow::onslideValueChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_5_clicked()
{
    ui -> lineEdit->setText("정지");
}

void MainWindow::on_pushButton_clicked()
{
    ui -> lineEdit->setText("전진");
}

void MainWindow::on_pushButton_2_clicked()
{
    ui -> lineEdit->setText("좌회전");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui -> lineEdit->setText("우회전");
}


void MainWindow::on_pushButton_4_clicked()
{
    ui -> lineEdit->setText("후진");
}

void MainWindow::onslideValueChanged(int value)
{
    ui->valueLabel->setText(QString::number(value));
}

