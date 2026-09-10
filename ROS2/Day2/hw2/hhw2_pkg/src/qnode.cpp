#include "../include/hhw2_pkg/qnode.hpp"

#include <chrono>
#include <thread>
#include <future>


using namespace std::chrono_literals;



QNode::QNode()
{
    int argc = 0;
    char** argv = nullptr;

    rclcpp::init(argc, argv);


    // ROS2 Node 생성
    node = rclcpp::Node::make_shared("hhw2_pkg");


    // cmd_vel Publisher 생성
    cmd_vel_publisher =
        node->create_publisher<geometry_msgs::msg::Twist>(
            "/turtle1/cmd_vel",
            10
            );


    // set_pen Service Client 생성
    pen_client =
        node->create_client<turtlesim::srv::SetPen>(
            "/turtle1/set_pen"
            );


    // QThread 시작
    this->start();
}


QNode::~QNode()
{
    if (rclcpp::ok())
    {
        rclcpp::shutdown();
    }

    // Thread가 끝날 때까지 기다림
    wait();
}


void QNode::run()
{
    rclcpp::WallRate loop_rate(20);


    while (rclcpp::ok())
    {
        // ROS2 callback 처리
        rclcpp::spin_some(node);

        loop_rate.sleep();
    }


    Q_EMIT rosShutDown();
}


void QNode::sendVelocity(
    double linear,
    double angular
    )
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


    // Publish
    cmd_vel_publisher->publish(msg);
}

void QNode::moveFor(
    double linear,
    double angular,
    int milliseconds
    )
{
    // 움직임 시작
    sendVelocity(
        linear,
        angular
        );


    // 해당 시간 동안 유지
    std::this_thread::sleep_for(
        std::chrono::milliseconds(milliseconds)
        );


    // 정지
    stop();


    // 다음 명령 전 약간 대기
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)
        );
}


void QNode::stop()
{
    sendVelocity(
        0.0,
        0.0
        );
}




void QNode::setPen(
    int r,
    int g,
    int b,
    int width
    )
{
    // turtlesim 서비스가 있는지 확인
    if (!pen_client->wait_for_service(1s))
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "set_pen 서비스를 찾을 수 없습니다."
            );

        return;
    }


    auto request =
        std::make_shared<turtlesim::srv::SetPen::Request>();


    request->r = r;
    request->g = g;
    request->b = b;

    request->width = width;

    // 0 = 펜 ON
    request->off = 0;


    auto future =
        pen_client->async_send_request(request);


    // 서비스 응답 잠깐 기다림
    future.wait_for(1s);
}


void QNode::drawSquare()
{
    RCLCPP_INFO(
        node->get_logger(),
        "사각형 그리기 시작"
        );


    for (int i = 0; i < 4; i++)
    {


        moveFor(
            2.0,       // 직진 속도
            0.0,       // 회전 없음
            1000       // 1초
            );




        moveFor(
            0.0,
            1.5708,    // 약 90도 / 초
            1000
            );
    }


    stop();


    RCLCPP_INFO(
        node->get_logger(),
        "사각형 완료"
        );
}



void QNode::drawTriangle()
{
    RCLCPP_INFO(
        node->get_logger(),
        "삼각형 그리기 시작"
        );


    for (int i = 0; i < 3; i++)
    {
        // 직진
        moveFor(
            2.0,
            0.0,
            1000
            );


        // 120도 회전
        moveFor(
            0.0,
            2.0944,
            1000
            );
    }


    stop();


    RCLCPP_INFO(
        node->get_logger(),
        "삼각형 완료"
        );
}


void QNode::drawCircle()
{
    RCLCPP_INFO(
        node->get_logger(),
        "원 그리기 시작"
        );


    for(int i = 0; i < 6; i++){
        moveFor(
            2.0,
            1.0,
            1000
            );
    }



    stop();


    RCLCPP_INFO(
        node->get_logger(),
        "원 완료"
        );
}



void QNode::drawPentagon()
{
    RCLCPP_INFO(
        node->get_logger(),
        "오각형 그리기 시작"
        );


    for (int i = 0; i < 5; i++)
    {
        // 직진
        moveFor(
            2.0,
            0.0,
            1000
            );


        // 72도 회전
        moveFor(
            0.0,
            1.2566,
            1000
            );
    }


    stop();


    RCLCPP_INFO(
        node->get_logger(),
        "오각형 완료"
        );
}
