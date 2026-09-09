#ifndef PUBLISHER_HPP_
#define PUBLISHER_HPP_

#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/my_message.hpp"

class HelloworldPublisher : public rclcpp::Node
{
public:
  HelloworldPublisher();

private:
  void publish_vector_msg();

  rclcpp::TimerBase::SharedPtr timer_; // 타이머 선언

  rclcpp::Publisher<custom_interfaces::msg::MyMessage>::SharedPtr vector_publisher_; // 퍼블리셔 선언

  //size_t count_;
};

#endif  // PUBLISHER_HPP_