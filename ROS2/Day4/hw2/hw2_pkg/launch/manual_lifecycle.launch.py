from launch import LaunchDescription
from launch_ros.actions import LifecycleNode


def generate_launch_description():

    publisher = LifecycleNode(
        package='hw2_pkg',
        executable='lifecycle_publisher',
        name='lifecycle_publisher',
        namespace='',
        output='screen'
    )

    subscriber = LifecycleNode(
        package='hw2_pkg',
        executable='lifecycle_subscriber',
        name='lifecycle_subscriber',
        namespace='',
        output='screen'
    )

    return LaunchDescription([
        publisher,
        subscriber
    ])