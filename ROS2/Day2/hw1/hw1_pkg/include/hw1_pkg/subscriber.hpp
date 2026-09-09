#ifndef SUBSCRIBER_HPP_
#define SUBSCRIBER_HPP_

#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/my_message.hpp"


class HelloworldSubscriber : public rclcpp::Node
{
public:
  HelloworldSubscriber();

private:
  void subscribe_vector_message(
    const custom_interfaces::msg::MyMessage::SharedPtr msg
  ) const;

  rclcpp::Subscription<custom_interfaces::msg::MyMessage>::SharedPtr
    vector_subscriber_;
};

#endif