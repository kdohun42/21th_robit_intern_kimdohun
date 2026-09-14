#ifndef LIFECYCLE_QOS_PKG__LIFECYCLE_PUBLISHER_HPP_
#define LIFECYCLE_QOS_PKG__LIFECYCLE_PUBLISHER_HPP_

#include <cstddef>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"


class LifecyclePublisherNode
  : public rclcpp_lifecycle::LifecycleNode
{
public:
  // Lifecycle callback의 반환 자료형을 짧게 사용하기 위한 별칭
  using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::
    LifecycleNodeInterface::CallbackReturn;

  // 생성자
  LifecyclePublisherNode();

  // Unconfigured -> Inactive
  CallbackReturn on_configure(
    const rclcpp_lifecycle::State & state) override;

  // Inactive -> Active
  CallbackReturn on_activate(
    const rclcpp_lifecycle::State & state) override;

  // Active -> Inactive
  CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & state) override;

  // Inactive -> Unconfigured
  CallbackReturn on_cleanup(
    const rclcpp_lifecycle::State & state) override;

private:
  // 실제 메시지를 보내는 함수
  void publish_data();

  // Lifecycle Publisher
  std::shared_ptr<
    rclcpp_lifecycle::LifecyclePublisher<
      std_msgs::msg::String>>
    publisher_;

  // 1초마다 publish_data() 실행
  rclcpp::TimerBase::SharedPtr timer_;

  // 메시지 번호
  std::size_t count_;
};

#endif  // hw2_PKG__LIFECYCLE_PUBLISHER_HPP_