#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"

using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:
    MinimalSubscriber()
    : Node("subscriber")
    {
        subscription_string_ =
            this->create_subscription<std_msgs::msg::String>(
                "topic_string",
                10,
                std::bind(
                    &MinimalSubscriber::topic_callback_string,
                    this,
                    _1));

        subscription_int32_ =
            this->create_subscription<std_msgs::msg::Int32>(
                "topic_int32",
                10,
                std::bind(
                    &MinimalSubscriber::topic_callback_int,
                    this,
                    _1));

        subscription_float32_ =
            this->create_subscription<std_msgs::msg::Float32>(
                "topic_float32",
                10,
                std::bind(
                    &MinimalSubscriber::topic_callback_float,
                    this,
                    _1));

        subscription_bool_ =
            this->create_subscription<std_msgs::msg::Bool>(
                "topic_bool",
                10,
                std::bind(
                    &MinimalSubscriber::topic_callback_bool,
                    this,
                    _1));
    }

private:

    void topic_callback_string(
        const std_msgs::msg::String & msg) const
    {
        RCLCPP_INFO(
            this->get_logger(),
            "String subscriber: '%s'",
            msg.data.c_str());
    }

    void topic_callback_int(
        const std_msgs::msg::Int32 & msg) const
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Int subscriber: '%d'",
            msg.data);
    }

    void topic_callback_float(
        const std_msgs::msg::Float32 & msg) const
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Float subscriber: '%.2f'",
            msg.data);
    }

    void topic_callback_bool(
        const std_msgs::msg::Bool & msg) const
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Bool subscriber: '%s'",
            msg.data ? "true" : "false");
    }


    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr
        subscription_string_;

    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr
        subscription_int32_;

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr
        subscription_float32_;

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr
        subscription_bool_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<MinimalSubscriber>());

    rclcpp::shutdown();

    return 0;
}