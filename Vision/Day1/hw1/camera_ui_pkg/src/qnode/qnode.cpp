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

#include "camera_ui_pkg/qnode/qnode.hpp"
#include <functional>

QNode::QNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("camera_ui_pkg");
  image_subscriber_= node->create_subscription<sensor_msgs::msg::Image>("/camera/image",10, std::bind(&QNode::imageCallback, this, std::placeholders::_1));
  this->start();
}

QNode::~QNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
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

void QNode::imageCallback(sensor_msgs::msg::Image::SharedPtr msg){
  cv::Mat image = cv_bridge::toCvCopy(msg, "bgr8")->image;{
  // 뮤텍스
  std::lock_guard<std::mutex> lock(image_mutex_);
  current_image_ = image.clone();
  }
  RCLCPP_INFO_ONCE(node->get_logger(),"CAMERA IMAGE RECEIVED");
  Q_EMIT imageReceived(); // 
}

cv::Mat QNode::getCurrentImage()
{
    std::lock_guard<std::mutex> lock(image_mutex_);
    return current_image_.clone();
}
