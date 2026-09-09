#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"
using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
    MinimalPublisher()
    : Node("publisher"), count_(0)
    {
        publisher_string_ = this->create_publisher<std_msgs::msg::String>( "topic_string", 10);

        publisher_int32_ = this->create_publisher<std_msgs::msg::Int32>( "topic_int32", 10);

        publisher_float32_ = this->create_publisher<std_msgs::msg::Float32>( "topic_float32", 10);

        publisher_bool_ = this->create_publisher<std_msgs::msg::Bool>( "topic_bool", 10);

        timer_ = this->create_wall_timer(
            500ms,
            std::bind(&MinimalPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        // String 메시지
        auto message = std_msgs::msg::String();
        message.data = "Hello, world! " + std::to_string(count_);

        // Int32 메시지
        auto message2 = std_msgs::msg::Int32();
        message2.data = count_;

        // Float32 메시지
        auto message3 = std_msgs::msg::Float32();
        message3.data = 3.14;

        // Bool 메시지
        auto message4 = std_msgs::msg::Bool();
        message4.data = true;


        // 터미널 출력
        RCLCPP_INFO(this->get_logger(),"String publisher: '%s'", message.data.c_str());

        RCLCPP_INFO( this->get_logger(), "Int publisher: '%d'", message2.data);

        RCLCPP_INFO( this->get_logger(), "Float publisher: '%.2f'",  message3.data);

        RCLCPP_INFO( this->get_logger(), "Bool publisher: '%s'", message4.data ? "true" : "false");


        // 각각의 토픽에 메시지 발행
        publisher_string_->publish(message);
        publisher_int32_->publish(message2);
        publisher_float32_->publish(message3);
        publisher_bool_->publish(message4);

        count_++;
    }

    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_string_;

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_int32_;

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_float32_;

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_bool_;

    size_t count_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<MinimalPublisher>());

    rclcpp::shutdown();

    return 0;
}