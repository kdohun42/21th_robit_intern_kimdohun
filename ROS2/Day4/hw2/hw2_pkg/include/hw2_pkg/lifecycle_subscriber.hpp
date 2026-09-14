#ifndef HW2_PKG__LIFECYCLE_SUBSCRIBER_HPP_
#define HW2_PKG__LIFECYCLE_SUBSCRIBER_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

class LifecycleSubscriberNode
  : public rclcpp_lifecycle::LifecycleNode
{
public:
  using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::
    LifecycleNodeInterface::CallbackReturn;

  LifecycleSubscriberNode();

  CallbackReturn on_configure(
    const rclcpp_lifecycle::State & state) override;

  CallbackReturn on_activate(
    const rclcpp_lifecycle::State & state) override;

  CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & state) override;

  CallbackReturn on_cleanup(
    const rclcpp_lifecycle::State & state) override;

private:
  void topic_callback(
    const std_msgs::msg::String::SharedPtr msg);

  rclcpp::Subscription<
    std_msgs::msg::String>::SharedPtr subscriber_;

  bool is_active_;
};

#endif  // HW2_PKG__LIFECYCLE_SUBSCRIBER_HPP_