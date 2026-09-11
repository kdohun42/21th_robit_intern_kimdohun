#include "hw2_qt_pkg/vehicle_node.hpp"

#include <chrono>
#include <functional>
#include <memory>

VehicleNode::VehicleNode()
: Node("vehicle_node"),
  speed_(120.0),
  current_speed_(0.0),
  x_(700.0),
  start_x_(700.0),
  traffic_state_("RED")
{
  // 자동차 속도 파라미터
  this->declare_parameter<double>(
    "speed",
    120.0
  );

  // 자동차 시작 위치
  this->declare_parameter<double>(
    "start_x",
    700.0
  );

  speed_ =
    this->get_parameter("speed").as_double();

  start_x_ =
    this->get_parameter("start_x").as_double();

  x_ = start_x_;

  // 신호등 QoS
  rclcpp::QoS qos(
    rclcpp::KeepLast(1)
  );

  qos.reliable();
  qos.transient_local();

  // 신호등 Subscriber
  traffic_subscriber_ =
    this->create_subscription<std_msgs::msg::String>(
      "traffic_light",
      qos,
      std::bind(
        &VehicleNode::traffic_light_callback,
        this,
        std::placeholders::_1
      )
    );

  // 자동차 위치 Publisher
  position_publisher_ =
    this->create_publisher<std_msgs::msg::Float64>(
      "vehicle_position",
      10
    );

  // 50ms마다 자동차 위치 계산
  timer_ =
    this->create_wall_timer(
      std::chrono::milliseconds(50),
      std::bind(
        &VehicleNode::update_vehicle,
        this
      )
    );

  RCLCPP_INFO(
    this->get_logger(),
    "Vehicle Node Start"
  );

  RCLCPP_INFO(
    this->get_logger(),
    "Speed : %.1f",
    speed_
  );
}


void VehicleNode::traffic_light_callback(
  const std_msgs::msg::String::SharedPtr msg
)
{
  traffic_state_ = msg->data;

  RCLCPP_INFO(
    this->get_logger(),
    "Traffic : %s",
    traffic_state_.c_str()
  );
}


void VehicleNode::update_vehicle()
{
  // 50ms = 0.05초
  const double dt = 0.05;

  // GREEN
  if (traffic_state_ == "GREEN")
  {
    current_speed_ = speed_;
  }

  // YELLOW
  else if (traffic_state_ == "YELLOW")
  {
    current_speed_ -= speed_ * dt;

    if (current_speed_ < 0.0)
    {
      current_speed_ = 0.0;
    }
  }

  // RED
  else if (traffic_state_ == "RED")
  {
    current_speed_ = 0.0;
  }

  x_ -= current_speed_ * dt;

  // 화면 밖으로 나가면 오른쪽에서 다시 시작
  if (x_ < -100.0)
  {
    x_ = start_x_;
  }

  // 현재 위치 Publish
  auto msg =
    std_msgs::msg::Float64();

  msg.data = x_;

  position_publisher_->publish(msg);
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<VehicleNode>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}