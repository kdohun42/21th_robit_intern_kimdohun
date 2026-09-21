/**
 * @file /include/camera_ui_pkg/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef camera_ui_pkg_QNODE_HPP_
#define camera_ui_pkg_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>

/*****************************************************************************
** Class
*****************************************************************************/


class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();
  cv::Mat getCurrentImage();

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  void imageCallback(sensor_msgs::msg::Image::SharedPtr msg);
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscriber_;
  cv::Mat current_image_;
  std::mutex image_mutex_;

Q_SIGNALS:
  void rosShutDown();
  void imageReceived();
};

#endif /* camera_ui_pkg_QNODE_HPP_ */
