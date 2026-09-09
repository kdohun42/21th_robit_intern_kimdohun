#include <chrono>
#include <functional>
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstdint>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"