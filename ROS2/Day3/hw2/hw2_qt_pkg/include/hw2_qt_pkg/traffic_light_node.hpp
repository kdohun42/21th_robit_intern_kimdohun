#ifndef TRAFFIC_LIGHT_NODE_HPP_
#define TRAFFIC_LIGHT_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TrafficLightPublisher : public rclcpp::Node
{
public:
  TrafficLightPublisher();

private:
  void change_light();

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

  rclcpp::TimerBase::SharedPtr timer_;

  double time_;

  // 0 = RED
  // 1 = GREEN
  // 2 = YELLOW
  int state_;
};

#endif