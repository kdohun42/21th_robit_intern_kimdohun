#include "hw1_pkg/subscriber.hpp"

using std::placeholders::_1;


HelloworldSubscriber::HelloworldSubscriber()
: Node("vector_subscriber")
{
  auto qos_profile =
    rclcpp::QoS(rclcpp::KeepLast(10));

  vector_subscriber_ =
    this->create_subscription<custom_interfaces::msg::MyMessage>(
      "vector_topic",
      qos_profile,
      std::bind(
        &HelloworldSubscriber::subscribe_vector_message,
        this,
        _1
      )
    );
}

void HelloworldSubscriber::subscribe_vector_message(
  const custom_interfaces::msg::MyMessage::SharedPtr msg
) const
{
  RCLCPP_INFO(
    this->get_logger(),
    "Custom vector message received"
  );

  for (const auto & value : msg->data)
  {
    RCLCPP_INFO(
      this->get_logger(),
      "%d",
      value
    );
  }
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node =
    std::make_shared<HelloworldSubscriber>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}