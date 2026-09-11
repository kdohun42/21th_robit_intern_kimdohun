//cd ~/Desktop/colcon_ws
// colcon build --base-paths src/21th_robit_intern_kimdohun/ROS2/Day3/hw1
#include "hw1_pkg/hw1_node.hpp"
#include <chrono>
#include <cctype>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;


TurtleController::TurtleController()
: Node("turtle_controller")
{

  // cmd_vel Publisher
  cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>( "/turtle1/cmd_vel", 10 );

  // set_pen Service Client
  pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");

  // -----------------------------------------
  // 현재 펜 기본 설정
  //
  // YAML parameter로 변경할 값
  // -----------------------------------------
  //pen_r_ = 255;
  //pen_g_ = 0;
  //pen_b_ = 0;

  this->declare_parameter<int>("pen_r_", 0);
  this->declare_parameter<int>("pen_g_", 0);
  this->declare_parameter<int>("pen_b_", 0);
  this->declare_parameter<int>("pen_width_", 0);

  pen_r_ = this->get_parameter("pen_r_").as_int();
  pen_g_ = this->get_parameter("pen_g_").as_int();
  pen_b_ = this->get_parameter("pen_b_").as_int();
  pen_width_ = this->get_parameter("pen_width_").as_int();

   RCLCPP_INFO(
    this->get_logger(),
    "YAML parameter -> R:%d G:%d B:%d Width:%d",
    pen_r_,
    pen_g_,
    pen_b_,
    pen_width_
  );
  //pen_width_ = 3;
}

// 속도 명령 보내기

void TurtleController::sendVelocity( double linear, double angular)
{
  auto msg = geometry_msgs::msg::Twist();


  // 직진 속도
  msg.linear.x = linear;
  msg.linear.y = 0.0;
  msg.linear.z = 0.0;


  // 회전 속도
  msg.angular.x = 0.0;
  msg.angular.y = 0.0;
  msg.angular.z = angular;


  // ROS2 Publish
  cmd_vel_publisher_->publish(msg);


  RCLCPP_INFO(
    this->get_logger(),
    "cmd_vel -> linear.x = %.2f, angular.z = %.2f",
    linear,
    angular
  );
}

// 일정 시간 움직이기

void TurtleController::moveFor( double linear, double angular, int milliseconds)
{
  // 이동 시작
  sendVelocity( linear, angular);

  // 지정한 시간 동안 이동
  std::this_thread::sleep_for( std::chrono::milliseconds(milliseconds));

  // 정지
  stop();

  // 다음 명령 전 잠깐 대기
  std::this_thread::sleep_for( std::chrono::milliseconds(100)
  );
}

// 정지

void TurtleController::stop()
{
  sendVelocity( 0.0, 0.0 );
}

// 펜 설정

void TurtleController::setPen()
{
  // set_pen 서비스가 실행 중인지 확인
  if (!pen_client_->wait_for_service(2s))
  {
    RCLCPP_ERROR( this->get_logger(), "/turtle1/set_pen 서비스를 찾을 수 없습니다." );
    return;
  }

  auto request = std::make_shared<turtlesim::srv::SetPen::Request>();

  // 색상
  request->r = pen_r_;
  request->g = pen_g_;
  request->b = pen_b_;


  // 굵기
  request->width = pen_width_;


  // 0 = 펜 사용
  request->off = 0;


  auto future = pen_client_->async_send_request(request);


  if (
    future.wait_for(2s)
    == std::future_status::ready
  )
  {
    RCLCPP_INFO(
      this->get_logger(),
      "펜 설정 완료 R:%d G:%d B:%d Width:%d",
      pen_r_,
      pen_g_,
      pen_b_,
      pen_width_
    );
  }
  else
  {
    RCLCPP_WARN(
      this->get_logger(),
      "펜 설정 응답 대기 시간 초과"
    );
  }
}

// W : 사각형

void TurtleController::drawSquare()
{
  RCLCPP_INFO(
    this->get_logger(),
    "사각형 그리기 시작"
  );


  // 4개의 변
  for (int i = 0; i < 4; i++)
  {
    // 직진
    moveFor( 2.0, 0.0, 1000);

    // 90도 회전
    moveFor( 0.0, 1.5708, 1000);
  }

  stop();

  RCLCPP_INFO( this->get_logger(), "사각형 완료");
}

// A : 삼각형

void TurtleController::drawTriangle()
{
  RCLCPP_INFO(
    this->get_logger(),
    "삼각형 그리기 시작"
  );


  // 3개의 변
  for (int i = 0; i < 3; i++)
  {
    // 직진
    moveFor( 2.0, 0.0, 1000);
    // 120도 회전
    moveFor( 0.0, 2.0944, 1000);
  }

  stop();

  RCLCPP_INFO( this->get_logger(), "삼각형 완료");
}

// S : 원

void TurtleController::drawCircle()
{
  RCLCPP_INFO(this->get_logger(), "원 그리기 시작" );

  for(int i = 0; i < 7; i++){
    moveFor(2.0, 1.0, 1000 );
  }

  stop();

  RCLCPP_INFO( this->get_logger(), "원 완료");
}

// D : 오각형

void TurtleController::drawPentagon()
{
  RCLCPP_INFO( this->get_logger(), "오각형 그리기 시작");

  for (int i = 0; i < 5; i++)
  {
    // 직진
    moveFor( 2.0, 0.0, 1000);
    // 72도 회전
    moveFor( 0.0, 1.2566, 1000);
  }

  stop();
  RCLCPP_INFO( this->get_logger(), "오각형 완료" );
}


// =====================================================
// 입력값에 따른 도형 선택
// =====================================================

void TurtleController::drawShape( char input )
{
  // 소문자가 들어와도 대문자로 변경
  input = std::toupper(input);

  // 먼저 펜 설정
  setPen();

  // W
  if (input == 'W')
  {
    drawSquare();
  }

  // A
  else if (input == 'A')
  {
    drawTriangle();
  }

  // S
  else if (input == 'S')
  {
    drawCircle();
  }

  // D
  else if (input == 'D')
  {
    drawPentagon();
  }

  else if (input == 'q')
  {
    rclcpp::shutdown();
   }
  // 잘못된 입력
  else
  {
    RCLCPP_ERROR( this->get_logger(), "잘못된 입력입니다. W/A/S/D 중 하나를 입력하세요.");
  }

}


// =====================================================
// main
// =====================================================

int main( int argc, char* argv[])
{ 
  // ROS2 초기화
  rclcpp::init( argc, argv );

  // TurtleController Node 생성
  auto node = std::make_shared<TurtleController>();

  // ROS2 callback 처리용 Thread
  std::thread ros_thread( [node]() { rclcpp::spin(node);});

  while(1){
  // 입력
  char input;
  std::cout << "W : 사각형\n" << "A : 삼각형\n" << "S : 원\n" << "D : 오각형\n" << "입력 : ";
  std::cin >> input;

  // 도형 그리기
  node->drawShape(input);
  }

  // ROS2 종료

  //rclcpp::shutdown();


  if (ros_thread.joinable())
  {
    ros_thread.join();
  }

  return 0;
}