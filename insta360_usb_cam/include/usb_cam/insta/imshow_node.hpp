#ifndef IMSHOW_NODE_HPP_
#define IMSHOW_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <std_msgs/msg/header.hpp>
#include <memory>
#include <string>

using namespace cv;
using namespace std;

class imshowNode : public rclcpp::Node
{
public:
    imshowNode();

private:
    void imageCallback(sensor_msgs::msg::Image::SharedPtr msg);
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscriber_;
};

#endif