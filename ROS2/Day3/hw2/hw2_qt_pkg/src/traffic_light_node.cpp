#include "hw2_qt_pkg/traffic_light_node.hpp"

#include <chrono>
#include <functional>
#include <memory>


TrafficLightPublisher::TrafficLightPublisher()
: Node("traffic_light_node"),
  time_(5.0),
  state_(0)
{
  // 신호 유지 시간 파라미터
  this->declare_parameter<double>("time", 5.0);

  time_ =
    this->get_parameter("time").as_double();


  if (time_ <= 0.0)
  {
    time_ = 5.0;
  }

  rclcpp::QoS qos(rclcpp::KeepLast(1));

  qos.reliable();
  qos.transient_local();


  publisher_ =
    this->create_publisher<std_msgs::msg::String>(
      "traffic_light",
      qos
    );


  // 프로그램 시작과 동시에 RED
  change_light();


  // time_초마다 신호 변경
  timer_ =
    this->create_wall_timer(
      std::chrono::milliseconds(
        static_cast<int>(time_ * 1000)
      ),

      std::bind(
        &TrafficLightPublisher::change_light,
        this
      )
    );
}


void TrafficLightPublisher::change_light()
{
  auto msg = std_msgs::msg::String();


  if (state_ == 0)
  {
    msg.data = "RED";

    state_ = 1;
  }

  else if (state_ == 1)
  {
    msg.data = "GREEN";

    state_ = 2;
  }

  else
  {
    msg.data = "YELLOW";

    state_ = 0;
  }


  publisher_->publish(msg);


  RCLCPP_INFO(
    this->get_logger(),
    "Traffic Light : %s",
    msg.data.c_str()
  );
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);


  auto node =
    std::make_shared<TrafficLightPublisher>();


  rclcpp::spin(node);


  rclcpp::shutdown();

  return 0;
}