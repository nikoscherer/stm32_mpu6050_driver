cmake_minimum_required(VERSION 3.8)
project(robot_firmware)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(listener src/stm32_interface_node.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

install(TARGETS listener
  DESTINATION lib/${PROJECT_NAME})

ament_package()