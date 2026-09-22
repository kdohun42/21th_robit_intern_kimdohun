#include "usb_cam/insta/imshow_node.hpp"

#include <chrono>
#include <functional>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;


double W = 0.06; // 공의 지름 6CM 
double fx = 471.953641;
double fy = 476.574144;
double cx = 309.509126;
double cy = 228.222101;
// 20도 라디안 값 변환
double theta = (20 * 3.141592) / 180.0;
double H = 0.29; // 높이

imshowNode::imshowNode()
: Node("imshow_node")
{
    subscriber_ = this->create_subscription<sensor_msgs::msg::Image>("/camera1/camera/compressed_image",10, std::bind(&imshowNode::imageCallback, this, std::placeholders::_1 ));
    RCLCPP_INFO(this->get_logger(),"YES CAMERA OPEN");
}


void imshowNode::imageCallback(sensor_msgs::msg::Image::SharedPtr msg){
    // 노란색 공 이진 마스크 만들기 및 바운딩 박스 그리기
    Mat image = cv_bridge::toCvCopy(msg, "bgr8")->image;
    Mat image2, mask, yellow, yellow_detection;
    cvtColor(image, image2, COLOR_BGR2HSV);
    inRange(image2,Scalar(20,100,100), Scalar(30,255,255), mask);
    bitwise_and(image,image, yellow, mask);
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty())
    {
    imshow("yellow_detection", image);
    waitKey(1);
    return;
    }

    Rect box = boundingRect(contours[0]);
    rectangle(yellow, box, Scalar(0, 255, 255), 2);
    bitwise_or(yellow, image, yellow_detection);

    // 바운딩 박스의 x, y, width, height
    double x = box.x;
    double y = box.y;
    double w = box.width;
    double h = box.height;
    string text = "X:" + to_string(x) + " Y:" + to_string(y) + " W:" + to_string(w) + " H:" + to_string(h);
    putText(yellow_detection, text, Point(x, y -5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);


    double Distance1 = ((fx * W) / w) * 100;
    string text2 = "D1:" + to_string(Distance1);
    putText(yellow_detection, text2, Point(30, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);


    double v = box.y + box.height;
    double yn = (v-cy) / fy;
    double Distance2 = (H / tan(theta + atan((v-cy)/fy))) * 100;
    double tilt_angle = (atan(H/(Distance2/100) * 180)) / 3.141592; // 틸트 각
    double camera_angle = ((atan(H/(Distance2/100)) * 180) / 3.141592) - ((atan(yn) * 180) / 3.141592);

    string text3 = "D2:" + to_string(Distance2);
    putText(yellow_detection, text3, Point(30, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);

    string text4 = "TILT_ANGLE:" + to_string(tilt_angle);
    putText(yellow_detection, text4, Point(30, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);

    string text5 = "camera_ANGLE:" + to_string(camera_angle);
    putText(yellow_detection, text5, Point(30, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);

    imshow("yellow_detection", yellow_detection);
    waitKey(1);
}



int main(int ac, char** av) {
	rclcpp::init(ac, av);
	auto node = make_shared<imshowNode>(); 
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}


