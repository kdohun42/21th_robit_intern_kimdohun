#ifndef CAMERA_NODE_HPP_
#define CAMERA_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <std_msgs/msg/header.hpp>

#include <string>

using namespace cv;
using namespace std;

class CameraNode : public rclcpp::Node
{
public:
    CameraNode();

private:
    VideoCapture cap_;
    Mat frame_;
    void cameraCallback();
    //string camera_url_; // 카메라 주소 저장할 문자열
    int camera_index_;
    std::string topic; // 토픽 메세지 타입
    int time; // 프레임
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
};
#endif