#ifndef HW1_NODE_HPP_
#define HW1_NODE_HPP_

#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/twist.hpp>
#include <turtlesim/srv/set_pen.hpp>

#include <memory>


class TurtleController : public rclcpp::Node
{
public:
  TurtleController();

  // 입력받은 문자에 따라 도형 선택
  void drawShape(char input);
  // 사용할 파라미터 선언

private:
  // 거북이 속도 publish
  void sendVelocity( double linear, double angular);

  // 일정 시간 동안 움직이기
  void moveFor( double linear, double angular, int milliseconds);

  // 거북이 정지
  void stop();

  // 펜 설정
  void setPen();

  // 도형
  void drawSquare();
  void drawTriangle();
  void drawCircle();
  void drawPentagon();


  // cmd_vel Publisher
  rclcpp::Publisher<
    geometry_msgs::msg::Twist
  >::SharedPtr cmd_vel_publisher_;


  // set_pen Service Client
  rclcpp::Client<
    turtlesim::srv::SetPen
  >::SharedPtr pen_client_;

  int pen_r_;
  int pen_g_;
  int pen_b_;
  int pen_width_;
};

#endif