#ifndef VEHICLE_NODE_HPP_
#define VEHICLE_NODE_HPP_

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"

class VehicleNode : public rclcpp::Node
{
public:
  VehicleNode();

private:
  // 신호등 상태 받기
  void traffic_light_callback(
    const std_msgs::msg::String::SharedPtr msg
  );

  // 자동차 위치 계산
  void update_vehicle();

  // 신호등 Subscriber
  rclcpp::Subscription<
    std_msgs::msg::String
  >::SharedPtr traffic_subscriber_;

  // 자동차 x좌표 Publisher
  rclcpp::Publisher<
    std_msgs::msg::Float64
  >::SharedPtr position_publisher_;

  // 자동차 이동 Timer
  rclcpp::TimerBase::SharedPtr timer_;

  // 설정 속도
  double speed_;

  // 현재 속도
  double current_speed_;

  // 현재 x좌표
  double x_;

  // 시작 x좌표
  double start_x_;

  // RED / GREEN / YELLOW
  std::string traffic_state_;
};

#endif  // VEHICLE_NODE_HPP_