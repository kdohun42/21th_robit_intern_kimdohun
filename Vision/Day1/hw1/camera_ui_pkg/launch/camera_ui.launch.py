from launch import LaunchDescription
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    config_file = os.path.join(
        get_package_share_directory("camera_ui_pkg"),
        "config",
        "camera_ui.yaml"
    )

    camera_node = Node(
        package="camera_ui_pkg",
        executable="camera_node",
        output="screen",
        parameters=[config_file]
    )

    camera_ui = Node(
        package="camera_ui_pkg",
        executable="camera_ui_pkg",
        output="screen"
    )

    return LaunchDescription([
        camera_node,
        camera_ui
    ])