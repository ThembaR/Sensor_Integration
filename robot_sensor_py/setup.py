from setuptools import setup
from glob import glob
import os

package_name = 'robot_sensor_py'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],

    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),

        ('share/' + package_name, ['package.xml']),

        (os.path.join('share', package_name, 'launch'),
            glob('launch/*.py')),

        (os.path.join('share', package_name, 'config'),
            glob('config/*.yaml')),
    ],

    install_requires=['setuptools'],
    zip_safe=True,

    maintainer='themba',

    description='ROS2 Modbus TCP driver',

    entry_points={
        'console_scripts': [
            'modbus_driver = robot_sensor_py.modbus_driver:main',
        ],
    },
)