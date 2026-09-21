cmake_minimum_required(VERSION 3.5)

project(camera_ui_pkg)


# =========================================================
# C++ 설정
# =========================================================

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# =========================================================
# Qt 자동 처리
# =========================================================

# Q_OBJECT 자동 처리
set(CMAKE_AUTOMOC ON)

# .ui 파일 자동 처리
set(CMAKE_AUTOUIC ON)

# .qrc 파일 자동 처리
set(CMAKE_AUTORCC ON)

# mainwindow.ui 파일이 들어있는 위치
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)


# =========================================================
# ROS2 Dependencies
# =========================================================

find_package(ament_cmake REQUIRED)

find_package(rclcpp REQUIRED)

find_package(sensor_msgs REQUIRED)

find_package(std_msgs REQUIRED)

find_package(cv_bridge REQUIRED)


# =========================================================
# OpenCV
# =========================================================

find_package(OpenCV REQUIRED)


# =========================================================
# Boost
# =========================================================

find_package(
    Boost
    REQUIRED
    COMPONENTS thread
)


# =========================================================
# Qt5
# =========================================================

find_package(
    Qt5
    REQUIRED
    COMPONENTS
        Widgets
        Core
        Gui
        Network
)


# =========================================================
# Include 경로
# =========================================================

include_directories(
    include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OpenCV_INCLUDE_DIRS}
)


# =========================================================
# Qt UI + QNode 실행파일
#
# 실행:
# ros2 run camera_ui_pkg camera_ui_pkg
# =========================================================

add_executable(
    camera_ui_pkg

    # cpp
    src/main.cpp
    src/main_window.cpp
    src/qnode/qnode.cpp

    # hpp
    include/camera_ui_pkg/main_window.hpp
    include/camera_ui_pkg/qnode/qnode.hpp

    # Qt UI
    ui/mainwindow.ui

    # Qt Resource
    resources/images.qrc
)


# camera_ui_pkg에서 사용하는 ROS2 패키지
ament_target_dependencies(
    camera_ui_pkg

    rclcpp
    sensor_msgs
    std_msgs
    cv_bridge
)


# camera_ui_pkg에서 사용하는 일반 라이브러리
target_link_libraries(
    camera_ui_pkg

    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network

    Boost::thread

    ${OpenCV_LIBRARIES}
)


# =========================================================
# Camera Node 실행파일
#
# 실행:
# ros2 run camera_ui_pkg camera_node
# =========================================================

add_executable(
    camera_node

    src/camera_node/camera_node.cpp

    include/camera_ui_pkg/camera_node/camera_node.hpp
)


# camera_node에서 사용하는 ROS2 패키지
ament_target_dependencies(
    camera_node

    rclcpp
    sensor_msgs
    std_msgs
    cv_bridge
)


# camera_node에서 OpenCV 사용
target_link_libraries(
    camera_node

    ${OpenCV_LIBRARIES}
)


# =========================================================
# 실행파일 Install
# =========================================================

install(
    TARGETS

    camera_ui_pkg
    camera_node

    DESTINATION lib/${PROJECT_NAME}
)


# =========================================================
# Launch / Config Install
# =========================================================

install(
    DIRECTORY

    launch
    config

    DESTINATION share/${PROJECT_NAME}
)


# =========================================================
# Testing
# =========================================================

if(BUILD_TESTING)

    find_package(ament_lint_auto REQUIRED)

    ament_lint_auto_find_test_dependencies()

endif()


# =========================================================
# ROS2 Package
# =========================================================

ament_package()