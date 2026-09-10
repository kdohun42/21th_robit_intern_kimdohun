#ifndef hhw2_pkg_QNODE_HPP_
#define hhw2_pkg_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif

#include <QThread>

#include <geometry_msgs/msg/twist.hpp>
#include <turtlesim/srv/set_pen.hpp>

#include <memory>


class QNode : public QThread
{
    Q_OBJECT

public:

    QNode();
    ~QNode();

    // 펜 설정
    void setPen(int r, int g, int b, int width);

    // 도형 그리기
    void drawSquare();
    void drawTriangle();
    void drawCircle();
    void drawPentagon();


protected:

    void run();


private:

    // 속도 명령 전송
    void sendVelocity(double linear, double angular);

    // 일정 시간 동안 움직이기
    void moveFor(
        double linear,
        double angular,
        int milliseconds
        );

    // 정지
    void stop();


    // ROS2 Node
    std::shared_ptr<rclcpp::Node> node;


    // /turtle1/cmd_vel
    rclcpp::Publisher<
        geometry_msgs::msg::Twist
        >::SharedPtr cmd_vel_publisher;


    // /turtle1/set_pen
    rclcpp::Client<
        turtlesim::srv::SetPen
        >::SharedPtr pen_client;


Q_SIGNALS:

    void rosShutDown();
};

#endif
