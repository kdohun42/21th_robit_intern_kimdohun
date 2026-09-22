from launch import LaunchDescription
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os
import yaml


def generate_launch_description():

    # ============================================================
    # Config 파일 경로
    # ============================================================

    config_dir = os.path.join(
        get_package_share_directory('insta360_usb_cam'),
        'config',
        'camera_config.yaml'
    )

    info_dir = os.path.join(
        get_package_share_directory('insta360_usb_cam'),
        'config',
        'camera_info_config.yaml'
    )


    # ============================================================
    # camera_name 읽기
    # ============================================================

    with open(config_dir, 'r') as file:
        config_params = yaml.safe_load(file)

        camera_name = \
            config_params['/**']['ros__parameters']['camera_name']


    node_name = f'pan_tilt_camera_node_{camera_name}'


    # ============================================================
    # Insta360 Camera Node
    # ============================================================

    camera_node = Node(
        package='insta360_usb_cam',
        executable='pan_tilt_camera_node',
        name=node_name,
        output='screen',

        parameters=[
            config_dir,
            info_dir
        ]
    )


    # ============================================================
    # OpenCV Image Processing Node
    # ============================================================

    imshow_node = Node(
        package='insta360_usb_cam',
        executable='imshow_node',
        name='imshow_node',
        output='screen'
    )


    # ============================================================
    # Launch
    # ============================================================

    return LaunchDescription([
        camera_node,
        imshow_node
    ])