#include "hw2_pkg/lifecycle_subscriber.hpp"

#include <functional>
#include <memory>


// 생성자
LifecycleSubscriberNode::LifecycleSubscriberNode()
: LifecycleNode("lifecycle_subscriber"),
  is_active_(false)
{
  RCLCPP_INFO(
    this->get_logger(),
    "Lifecycle Subscriber  : Unconfigured");
}


// configure 실행
LifecycleSubscriberNode::CallbackReturn
LifecycleSubscriberNode::on_configure(
  const rclcpp_lifecycle::State &)
{
  // Publisher와 같은 QoS 사용
  rclcpp::QoS qos(rclcpp::KeepLast(5));

  qos.best_effort();
  qos.durability_volatile();

  // Subscriber 생성
  subscriber_ =
    this->create_subscription<std_msgs::msg::String>(
      "data",
      qos,
      std::bind(
        &LifecycleSubscriberNode::topic_callback,
        this,
        std::placeholders::_1));

  RCLCPP_INFO(
    this->get_logger(),
    "on_configure() : Inactive");

  return CallbackReturn::SUCCESS;
}


// activate 실행
LifecycleSubscriberNode::CallbackReturn
LifecycleSubscriberNode::on_activate(
  const rclcpp_lifecycle::State & state)
{
  rclcpp_lifecycle::LifecycleNode::on_activate(state);

  is_active_ = true;

  RCLCPP_INFO(
    this->get_logger(),
    "on_activate() : Active");

  return CallbackReturn::SUCCESS;
}


// deactivate 실행
LifecycleSubscriberNode::CallbackReturn
LifecycleSubscriberNode::on_deactivate(
  const rclcpp_lifecycle::State & state)
{
  rclcpp_lifecycle::LifecycleNode::on_deactivate(state);

  is_active_ = false;

  RCLCPP_INFO(
    this->get_logger(),
    "on_deactivate() : Inactive");

  return CallbackReturn::SUCCESS;
}


// cleanup 실행
LifecycleSubscriberNode::CallbackReturn
LifecycleSubscriberNode::on_cleanup(
  const rclcpp_lifecycle::State &)
{
  subscriber_.reset();

  is_active_ = false;

  RCLCPP_INFO(
    this->get_logger(),
    "on_cleanup() : Unconfigured");

  return CallbackReturn::SUCCESS;
}


// 메시지를 받을 때 실행
void LifecycleSubscriberNode::topic_callback(
  const std_msgs::msg::String::SharedPtr msg)
{
  // Active 상태가 아니면 받은 데이터 처리 X
  if (!is_active_) {
    return;
  }

  RCLCPP_INFO(
    this->get_logger(),
    "Received : %s",
    msg->data.c_str());
}


// main
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<LifecycleSubscriberNode>();

  rclcpp::executors::SingleThreadedExecutor executor;

  executor.add_node(
    node->get_node_base_interface());

  executor.spin();

  rclcpp::shutdown();

  return 0;
}