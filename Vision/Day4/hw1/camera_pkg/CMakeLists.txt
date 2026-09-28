cmake_minimum_required(VERSION 3.8)
project(camera_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

# camera_node executable
add_executable(
  camera_node
  src/camera_node/camera_node.cpp
)

# Header 경로
target_include_directories(
  camera_node
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${OpenCV_INCLUDE_DIRS}
)

# ROS2 dependencies
ament_target_dependencies(
  camera_node
  rclcpp
  sensor_msgs
  cv_bridge
)

# OpenCV
target_link_libraries(
  camera_node
  ${OpenCV_LIBS}
)

# 실행 파일 설치
install(
  TARGETS
  camera_node
  DESTINATION lib/${PROJECT_NAME}
)

# Header 설치
install(
  DIRECTORY include/
  DESTINATION include
)

ament_package()