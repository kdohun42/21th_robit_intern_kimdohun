# Set minimum required version of cmake, project name and compile options
cmake_minimum_required(VERSION 3.5)
project(hw1_pkg)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_interfaces REQUIRED)

# Build
add_executable(publisher src/publisher.cpp)

ament_target_dependencies( 
      publisher 
      rclcpp 
      std_msgs
      custom_interfaces
)

target_include_directories(
  publisher PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(subscriber src/subscriber.cpp)

ament_target_dependencies(
      subscriber  
      rclcpp 
      std_msgs
      custom_interfaces
)

target_include_directories(
  subscriber PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Install
install(TARGETS
  publisher
  subscriber
  DESTINATION lib/${PROJECT_NAME})

# Test
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

# Macro for ament package
ament_package()
