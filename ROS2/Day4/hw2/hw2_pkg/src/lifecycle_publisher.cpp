#include "hw2_pkg/lifecycle_publisher.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <string>


using namespace std::chrono_literals;


// 생성자
LifecyclePublisherNode::LifecyclePublisherNode()
: LifecycleNode("lifecycle_publisher"),
  count_(0)
{
  RCLCPP_INFO(
    this->get_logger(),
    "Lifecycle Publisher : Unconfigured");
}


// configure가 실행될 때 호출
LifecyclePublisherNode::CallbackReturn
LifecyclePublisherNode::on_configure(
  const rclcpp_lifecycle::State &)
{
  // QoS 설정
  rclcpp::QoS qos(rclcpp::KeepLast(5));

  qos.best_effort();
  qos.durability_volatile();

  // Lifecycle Publisher 생성
  publisher_ =
    this->create_publisher<std_msgs::msg::String>(
      "data",
      qos);

  // 1초마다 publish_data() 실행
  timer_ =
    this->create_wall_timer(
      1s,
      std::bind(
        &LifecyclePublisherNode::publish_data,
        this));

  RCLCPP_INFO(
    this->get_logger(),
    "on_configure() : Inactive");

  return CallbackReturn::SUCCESS;
}


// activate가 실행될 때 호출
LifecyclePublisherNode::CallbackReturn
LifecyclePublisherNode::on_activate(
  const rclcpp_lifecycle::State & state)
{
  // Lifecycle Publisher도 활성화
  rclcpp_lifecycle::LifecycleNode::on_activate(state);

  RCLCPP_INFO(
    this->get_logger(),
    "on_activate() : Active");

  return CallbackReturn::SUCCESS;
}


// deactivate가 실행될 때 호출
LifecyclePublisherNode::CallbackReturn
LifecyclePublisherNode::on_deactivate(
  const rclcpp_lifecycle::State & state)
{
  // Lifecycle Publisher 비활성화
  rclcpp_lifecycle::LifecycleNode::on_deactivate(state);

  RCLCPP_INFO(
    this->get_logger(),
    "on_deactivate() : Inactive");

  return CallbackReturn::SUCCESS;
}


// cleanup이 실행될 때 호출
LifecyclePublisherNode::CallbackReturn
LifecyclePublisherNode::on_cleanup(
  const rclcpp_lifecycle::State &)
{
  // configure에서 만들었던 자원 제거
  timer_.reset();
  publisher_.reset();

  count_ = 0;

  RCLCPP_INFO(
    this->get_logger(),
    "on_cleanup() : Unconfigured");

  return CallbackReturn::SUCCESS;
}


// 실제 메시지 Publish
void LifecyclePublisherNode::publish_data()
{
  // Publisher가 없으면 종료
  if (!publisher_) {
    return;
  }

  // Active 상태가 아니라면 메시지를 보내지 않음
  if (!publisher_->is_activated()) {
    return;
  }

  std_msgs::msg::String msg;

  msg.data =
    "sample " + std::to_string(count_);

  count_++;

  publisher_->publish(msg);

  RCLCPP_INFO(
    this->get_logger(),
    "Publish : %s",
    msg.data.c_str());
}


// main
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<LifecyclePublisherNode>();

  rclcpp::executors::SingleThreadedExecutor executor;

  executor.add_node(
    node->get_node_base_interface());

  executor.spin();

  rclcpp::shutdown();

  return 0;
}