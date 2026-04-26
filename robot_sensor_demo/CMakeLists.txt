cmake_minimum_required(VERSION 3.8)
project(robot_sensor_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(distance_sensor_driver src/distance_sensor_driver.cpp)
ament_target_dependencies(distance_sensor_driver rclcpp std_msgs)

add_executable(obstacle_avoidance_node src/obstacle_avoidance_node.cpp)
ament_target_dependencies(obstacle_avoidance_node rclcpp std_msgs geometry_msgs)

install(TARGETS
  distance_sensor_driver
  obstacle_avoidance_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)


ament_package()