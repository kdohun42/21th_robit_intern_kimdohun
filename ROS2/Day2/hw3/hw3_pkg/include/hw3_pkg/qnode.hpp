#ifndef QNODE_HPP_
#define QNODE_HPP_

#include <QThread>
#include <QString>

#include <memory>
#include <string>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class QNode : public QThread
{
    Q_OBJECT

public:
    QNode();
    ~QNode();

    // MainWindow가 호출하는 함수
    // 전달받은 문자열을 ROS2 Topic으로 Publish
    void publishMessage(const std::string &text);

protected:
    // QThread의 실제 실행 함수
    void run();

Q_SIGNALS:
    // 기존 Signal
    void rosShutDown();

    // Subscriber가 메시지를 받았을 때
    // MainWindow로 문자열을 전달하기 위한 Signal
    void messageReceived(QString message);

private:
    // Subscriber가 메시지를 받으면 실행되는 Callback
    void messageCallback(
        const std_msgs::msg::String::SharedPtr msg
        );

    // ROS2 Node
    rclcpp::Node::SharedPtr node;

    // Publisher
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    // Subscriber
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};

#endif
