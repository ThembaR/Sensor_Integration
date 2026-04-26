from launch import LaunchDescription
from launch_ros.actions import Node
import os

def generate_launch_description():
    config = os.path.join(
        os.getenv('AMENT_PREFIX_PATH').split(':')[0],
        'share',
        'robot_sensor_demo',
        'config',
        'params.yaml'
    )

    return LaunchDescription([
        Node(
            package='robot_sensor_demo',
            executable='distance_sensor_driver',
            parameters=[config]
        ),
        Node(
            package='robot_sensor_demo',
            executable='obstacle_avoidance_node',
            parameters=[config]
        )
    ])