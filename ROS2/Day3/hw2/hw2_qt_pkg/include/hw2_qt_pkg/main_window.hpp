#ifndef HW2_QT_PKG_MAIN_WINDOW_HPP_
#define HW2_QT_PKG_MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QCloseEvent>
#include <QString>

#include "qnode.hpp"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT


public:
  MainWindow(
    QWidget* parent = nullptr
  );

  ~MainWindow();


private slots:
  // 신호등 변경
  void traffic_light(
    QString light
  );


  // 자동차 위치 변경
  void vehicle_position(
    int x
  );


private:
  void traffic_light_off();


  void closeEvent(
    QCloseEvent* event
  ) override;


  Ui::MainWindowDesign* ui;

  QNode* qnode;
};


#endif