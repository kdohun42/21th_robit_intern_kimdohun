#include "camera_ui_pkg/camera_node/camera_node.hpp"

#include <chrono>
#include <functional>

using namespace cv;
using namespace std;

// 카메라 노드의 생성자
CameraNode::CameraNode()
: Node("camera_node")
{	
	//camera_index_ = this->declare_parameter<int>("camera_index", 0);
	topic= this->declare_parameter<std::string>("topic", "/camera/image");
    time = this->declare_parameter<int>("timer",33);
	cap_.open(0);

	if(!cap_.isOpened()){
		RCLCPP_ERROR(this->get_logger(), "NO CAMERA OPEN");
		return;
	}
	RCLCPP_INFO(this->get_logger(),"YES CAMERA OPEN");
	// 타이머 생성
	image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/camera/image",10);
	timer_ = this->create_wall_timer(chrono::milliseconds(33), bind(&CameraNode::cameraCallback, this));
}

// 콜백 함수
void CameraNode::cameraCallback(){
	cap_ >> frame_;
	if(frame_.empty()){
		return;
	}
	// bgr 8비트 이미지를 ROS Image로 변환
	auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame_).toImageMsg();
	image_publisher_->publish(*msg);
}

int main(int ac, char** av) {
	rclcpp::init(ac, av);
	auto node = make_shared<CameraNode>(); 
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}