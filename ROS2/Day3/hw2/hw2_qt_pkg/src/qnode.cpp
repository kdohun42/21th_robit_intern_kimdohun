#include "../include/hw2_qt_pkg/qnode.hpp"

#include <functional>


QNode::QNode()
{
  int argc = 0;

  char** argv = nullptr;


  rclcpp::init(
    argc,
    argv
  );


  node =
    rclcpp::Node::make_shared(
      "hw2_qt_node"
    );




  rclcpp::QoS qos(
    rclcpp::KeepLast(1)
  );

  qos.reliable();

  qos.transient_local();


  traffic_subscriber_ =
    node->create_subscription<std_msgs::msg::String>(
      "traffic_light",

      qos,

      std::bind(
        &QNode::trafficLightCallback,
        this,
        std::placeholders::_1
      )
    );


  vehicle_subscriber_ =
    node->create_subscription<std_msgs::msg::Float64>(
      "vehicle_position",

      10,

      std::bind(
        &QNode::vehiclePositionCallback,
        this,
        std::placeholders::_1
      )
    );


  this->start();
}


void QNode::trafficLightCallback(
  const std_msgs::msg::String::SharedPtr msg
)
{
  emit trafficLightReceived(
    QString::fromStdString(
      msg->data
    )
  );
}


void QNode::vehiclePositionCallback(
  const std_msgs::msg::Float64::SharedPtr msg
)
{
  emit vehiclePositionReceived(
    static_cast<int>(
      msg->data
    )
  );
}


void QNode::run()
{
  rclcpp::WallRate loop_rate(50);


  while (
    rclcpp::ok() &&
    !isInterruptionRequested()
  )
  {
    rclcpp::spin_some(node);

    loop_rate.sleep();
  }


  Q_EMIT rosShutDown();
}


QNode::~QNode()
{
  requestInterruption();


  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }


  wait();
}