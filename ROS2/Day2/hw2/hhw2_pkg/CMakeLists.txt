cmake_minimum_required(VERSION 3.5)

project(hhw2_pkg)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ui 폴더에서 .ui 파일 찾기
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ROS2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)


# Boost
find_package(
    Boost 1.71.0
    REQUIRED
    COMPONENTS thread
)


# Qt5
find_package(
    Qt5
    REQUIRED
    COMPONENTS
    Widgets
    Core
    Gui
    Network
)


# cpp
file(
    GLOB PRO_SOURCE_DIR
    RELATIVE ${CMAKE_SOURCE_DIR}
    FOLLOW_SYMLINKS
    src/*.cpp
)


# ui
file(
    GLOB PRO_UI_DIR
    RELATIVE ${CMAKE_SOURCE_DIR}
    FOLLOW_SYMLINKS
    ui/*.ui
)


# header
file(
    GLOB PRO_INCLUDE_DIR
    RELATIVE ${CMAKE_SOURCE_DIR}
    FOLLOW_SYMLINKS
    include/${PROJECT_NAME}/*.h
    include/${PROJECT_NAME}/*.hpp
)


# resource
file(
    GLOB PRO_RESOURCE_DIR
    RELATIVE ${CMAKE_SOURCE_DIR}
    FOLLOW_SYMLINKS
    resource/*.qrc
)


# Qt MOC
qt5_wrap_cpp(
    QT_MOC_HPP
    ${PRO_INCLUDE_DIR}
)


# Qt Resource
qt5_add_resources(
    QT_RESOURCE_HPP
    ${PRO_RESOURCE_DIR}
)


include_directories(
    include/
    ${CMAKE_CURRENT_BINARY_DIR}
)


# 실행 파일
add_executable(
    ${PROJECT_NAME}

    ${PRO_SOURCE_DIR}
    ${PRO_INCLUDE_DIR}
    ${PRO_UI_DIR}

    ${QT_MOC_HPP}
    ${QT_RESOURCE_HPP}
)


# ROS2
ament_target_dependencies(
    ${PROJECT_NAME}

    rclcpp
    geometry_msgs
    turtlesim
)


# Qt
target_link_libraries(
    ${PROJECT_NAME}

    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network

    Boost::thread
)


install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib/${PROJECT_NAME}
)


if(BUILD_TESTING)

    find_package(ament_lint_auto REQUIRED)

    ament_lint_auto_find_test_dependencies()

endif()


ament_package()
