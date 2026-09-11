#ifndef HW2_QT_PKG_QNODE_HPP_
#define HW2_QT_PKG_QNODE_HPP_

#include <QThread>
#include <QString>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"


class QNode : public QThread
{
  Q_OBJECT


public:
  QNode();

  ~QNode();


protected:
  void run() override;


signals:
  void rosShutDown();


  // MainWindow로 신호 전달
  void trafficLightReceived(
    QString light
  );


  // MainWindow로 자동차 x좌표 전달
  void vehiclePositionReceived(
    int x
  );


private:
  void trafficLightCallback(
    const std_msgs::msg::String::SharedPtr msg
  );


  void vehiclePositionCallback(
    const std_msgs::msg::Float64::SharedPtr msg
  );


  rclcpp::Node::SharedPtr node;


  rclcpp::Subscription<
    std_msgs::msg::String
  >::SharedPtr traffic_subscriber_;


  rclcpp::Subscription<
    std_msgs::msg::Float64
  >::SharedPtr vehicle_subscriber_;
};


#endif