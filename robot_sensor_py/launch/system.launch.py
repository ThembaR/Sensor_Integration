from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():

    config = os.path.join(
        get_package_share_directory('robot_sensor_py'),
        'config',
        'params.yaml'
    )

    return LaunchDescription([

        # Python Modbus Driver
        Node(
            package='robot_sensor_py',
            executable='modbus_driver',
            name='modbus_driver',
            output='screen',
            parameters=[config]
        ),

        # C++ Obstacle Avoidance Node
        Node(
            package='robot_sensor_demo',
            executable='obstacle_avoidance_node',
            name='obstacle_avoidance_node',
            output='screen',
            parameters=[config]
        ),
    ])