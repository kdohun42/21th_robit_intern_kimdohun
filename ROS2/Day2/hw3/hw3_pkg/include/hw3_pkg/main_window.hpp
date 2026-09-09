/**
 * @file /include/hw3_pkg/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef hw3_pkg_MAIN_WINDOW_H
#define hw3_pkg_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include "qnode.hpp"
/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

  private slots:
  void on_pushButton_clicked();
  void updateReceivedMessage(QString message);

  void on_pushButton_2_clicked();

  private:
  Ui::MainWindowDesign* ui;
  void closeEvent(QCloseEvent* event);
};

#endif  // hw3_pkg_MAIN_WINDOW_H
