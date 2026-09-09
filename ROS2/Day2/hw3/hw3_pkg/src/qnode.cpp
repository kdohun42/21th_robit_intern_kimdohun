/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/hw3_pkg/qnode.hpp"

QNode::QNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("hw3_pkg");

  publisher_ = node->create_publisher<std_msgs::msg::String>( "gui_message", 10);

  subscriber_ = node->create_subscription<std_msgs::msg::String>( "gui_message", 10, std::bind(&QNode::messageCallback, this, std::placeholders::_1));


  this->start();
}

QNode::~QNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
}


void QNode::publishMessage(const std::string &text)
{
    auto msg = std_msgs::msg::String();

    msg.data = text;

    publisher_->publish(msg);

    RCLCPP_INFO(
        node->get_logger(),
        "Published: %s",
        msg.data.c_str()
        );
}

void QNode::messageCallback(
    const std_msgs::msg::String::SharedPtr msg
    )
{
    // 터미널 확인용
    RCLCPP_INFO(
        node->get_logger(),
        "Subscriber: %s",
        msg->data.c_str()
        );

    // std::string -> QString 변환 후
    // MainWindow에게 전달
    Q_EMIT messageReceived(
        QString::fromStdString(msg->data)
        );
}


void QNode::run()
{
  rclcpp::WallRate loop_rate(20);
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  Q_EMIT rosShutDown();
}


