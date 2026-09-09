/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/hw3_pkg/main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);
qnode = new QNode();
    connect(
        qnode,
        &QNode::messageReceived,
        this,
        &MainWindow::updateReceivedMessage
        );

  // ROS2가 종료되면 GUI 종료
  connect(
      qnode,
      &QNode::rosShutDown,
      this,
      &MainWindow::close
      );

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);



  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    qDebug() << "입력:" << text;
    qnode->publishMessage(
        text.toStdString());
}

void MainWindow::updateReceivedMessage(QString message)
{
    ui->label->setText(
        "Received : " + message
        );
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label->clear();
    ui->lineEdit->clear();
}

