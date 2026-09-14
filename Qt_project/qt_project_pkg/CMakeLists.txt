cmake_minimum_required(VERSION 3.5)

project(qt_project_pkg)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)


set(
    CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(
    ament_cmake
    REQUIRED
)


find_package(
    rclcpp
    REQUIRED
)


find_package(
    std_msgs
    REQUIRED
)


find_package(
    std_srvs
    REQUIRED
)


find_package(
    Boost
    1.71.0
    REQUIRED COMPONENTS
    thread
)


find_package(
    Qt5
    REQUIRED COMPONENTS
    Widgets
    Core
    Gui
    Network
)


set(PROJECT_SOURCES

    src/main.cpp

    src/main_window.cpp

    src/qnode.cpp

    src/zombie_ai_node.cpp

    src/map_manager.cpp

    src/building.cpp

    src/building_manager.cpp

    src/player.cpp

    src/zombie.cpp

    src/cat.cpp

    src/canned_food.cpp

    src/path_finder.cpp

    src/vision_manager.cpp

    src/torch.cpp

    src/game_renderer.cpp


    include/qt_project_pkg/main_window.hpp

    include/qt_project_pkg/qnode.hpp

    include/qt_project_pkg/zombie_ai_node.hpp

    include/qt_project_pkg/map_manager.hpp

    include/qt_project_pkg/building.hpp

    include/qt_project_pkg/building_manager.hpp

    include/qt_project_pkg/player.hpp

    include/qt_project_pkg/zombie.hpp

    include/qt_project_pkg/cat.hpp

    include/qt_project_pkg/canned_food.hpp

    include/qt_project_pkg/path_finder.hpp

    include/qt_project_pkg/vision_manager.hpp

    include/qt_project_pkg/torch.hpp

    include/qt_project_pkg/game_renderer.hpp


    ui/mainwindow.ui

    resources/images.qrc
)


add_executable(
    ${PROJECT_NAME}

    ${PROJECT_SOURCES}
)


target_include_directories(
    ${PROJECT_NAME}

    PRIVATE

    include
)


ament_target_dependencies(
    ${PROJECT_NAME}

    rclcpp

    std_msgs

    std_srvs
)


target_link_libraries(
    ${PROJECT_NAME}

    Qt5::Widgets

    Qt5::Core

    Qt5::Gui

    Qt5::Network

    Boost::thread
)


install(
    TARGETS

    ${PROJECT_NAME}

    DESTINATION

    lib/${PROJECT_NAME}
)


if(BUILD_TESTING)

    find_package(
        ament_lint_auto
        REQUIRED
    )

    ament_lint_auto_find_test_dependencies()

endif()


ament_package()