#include "hw1_pkg/publisher.hpp"

using namespace std::chrono_literals;


HelloworldPublisher::HelloworldPublisher()
: Node("vector_publisher")
{
  auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

  vector_publisher_ =
    this->create_publisher<custom_interfaces::msg::MyMessage>(
      "vector_topic",
      qos_profile
    );

  timer_ = this->create_wall_timer(
    1s,
    std::bind(&HelloworldPublisher::publish_vector_msg, this)
  );
}


void HelloworldPublisher::publish_vector_msg()
{
  auto msg = custom_interfaces::msg::MyMessage(); // 보낼 메세지 생성 Mymessage 객체 생성

  msg.data = {10, 20, 30, 40, 50};

  vector_publisher_->publish(msg); // 메세지를 보내는 함수 

  RCLCPP_INFO(
    this->get_logger(),
    "vector published"
  );
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<HelloworldPublisher>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}