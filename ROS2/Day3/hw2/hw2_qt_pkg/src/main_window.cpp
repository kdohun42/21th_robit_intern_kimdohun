#include "../include/hw2_qt_pkg/main_window.hpp"


MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent),
  ui(new Ui::MainWindowDesign),
  qnode(nullptr)
{
  ui->setupUi(this);


  // 신호등 처음에는 모두 OFF
  traffic_light_off();


  // QNode 생성
  qnode = new QNode();


  // ROS 종료
  QObject::connect(
    qnode,
    &QNode::rosShutDown,

    this,
    &MainWindow::close
  );


  // 신호등 Signal
  QObject::connect(
    qnode,
    &QNode::trafficLightReceived,

    this,
    &MainWindow::traffic_light
  );


  // 자동차 위치 Signal
  QObject::connect(
    qnode,
    &QNode::vehiclePositionReceived,

    this,
    &MainWindow::vehicle_position
  );
}


void MainWindow::traffic_light_off()
{
  ui->redLabel->setStyleSheet(
    "background-color: #404040;"
    "border-radius: 54px;"
  );


  ui->yellowLabel->setStyleSheet(
    "background-color: #404040;"
    "border-radius: 54px;"
  );


  ui->greenLabel->setStyleSheet(
    "background-color: #404040;"
    "border-radius: 54px;"
  );
}


void MainWindow::traffic_light(
  QString light
)
{
  // 일단 전부 OFF
  traffic_light_off();


  if (light == "RED")
  {
    ui->redLabel->setStyleSheet(
      "background-color: red;"
      "border-radius: 54px;"
    );
  }


  else if (light == "GREEN")
  {
    ui->greenLabel->setStyleSheet(
      "background-color: green;"
      "border-radius: 54px;"
    );
  }


  else if (light == "YELLOW")
  {
    ui->yellowLabel->setStyleSheet(
      "background-color: yellow;"
      "border-radius: 54px;"
    );
  }
}


void MainWindow::vehicle_position(
  int x
)
{
  // 현재 y좌표는 그대로 사용
  int y =
    ui->carLabel->y();


  // x좌표만 변경
  ui->carLabel->move(
    x,
    y
  );
}


void MainWindow::closeEvent(
  QCloseEvent* event
)
{
  QMainWindow::closeEvent(
    event
  );
}


MainWindow::~MainWindow()
{
  delete qnode;

  delete ui;
}