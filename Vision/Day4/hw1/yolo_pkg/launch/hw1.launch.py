from launch import LaunchDescription
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    # yolo_pkg의 share 경로 가져오기
    yolo_pkg_path = get_package_share_directory('yolo_pkg')

    # config.yaml 경로
    config_file = os.path.join(
        yolo_pkg_path,
        'config',
        'config.yaml'
    )

    # C++ Camera Node
    camera_node = Node(
        package='camera_pkg',
        executable='camera_node',
        name='camera_node',
        output='screen',
        parameters=[config_file]
    )

    # Python YOLO Node
    yolo_node = Node(
        package='yolo_pkg',
        executable='yolo_node',
        name='yolo_node',
        output='screen',
        parameters=[config_file]
    )

    return LaunchDescription([
        camera_node,
        yolo_node
    ])