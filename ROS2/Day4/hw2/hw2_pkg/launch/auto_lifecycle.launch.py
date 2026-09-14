from launch import LaunchDescription

from launch_ros.actions import LifecycleNode
from launch_ros.actions import LifecycleTransition

from lifecycle_msgs.msg import Transition


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

    auto_transition = LifecycleTransition(
        lifecycle_node_names=[
            'lifecycle_publisher',
            'lifecycle_subscriber'
        ],
        transition_ids=[
            Transition.TRANSITION_CONFIGURE,
            Transition.TRANSITION_ACTIVATE
        ]
    )

    return LaunchDescription([
        publisher,
        subscriber,
        auto_transition
    ])