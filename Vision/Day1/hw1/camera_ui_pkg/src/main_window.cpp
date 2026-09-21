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

#include "camera_ui_pkg/main_window.hpp"
#include "camera_ui_pkg/qnode/qnode.hpp"

#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  qnode = new QNode();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
  QObject::connect(
    qnode,
    &QNode::imageReceived,
    this,
    &MainWindow::updateCameraImage
);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateCameraImage(){
  cv::Mat image = qnode->getCurrentImage();
  if(image.empty()){
    return;
  }
  cv::cvtColor(image,image,cv::COLOR_BGR2RGB); // BGR을 RGB로 변환
  QImage qimage(image.data,image.cols,image.rows,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qimage));
}