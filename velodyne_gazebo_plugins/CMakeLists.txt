cmake_minimum_required(VERSION 3.0.2)
project(velodyne_gazebo_plugins)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  tf
  gazebo_ros
  pcl_conversions
  pcl_ros
  rospy
  std_msgs
)

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   location.msg
   radar_data.msg
   radar_data_extension.msg
)

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
)

find_package(gazebo REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

catkin_package(
  INCLUDE_DIRS include ${GAZEBO_INCLUDE_DIRS}
  LIBRARIES gazebo_ros_velodyne_laser gazebo_ros_velodyne_gpu_laser
  CATKIN_DEPENDS roscpp sensor_msgs gazebo_ros
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

link_directories(
  ${GAZEBO_LIBRARY_DIRS}
)

add_library(gazebo_ros_velodyne_laser src/GazeboRosVelodyneLaser.cpp)
target_link_libraries(gazebo_ros_velodyne_laser
  ${catkin_LIBRARIES}
  ${GAZEBO_LIBRARIES}
  RayPlugin
)

add_library(gazebo_ros_velodyne_gpu_laser src/GazeboRosVelodyneLaser.cpp)
target_link_libraries(gazebo_ros_velodyne_gpu_laser
  ${catkin_LIBRARIES}
  ${GAZEBO_LIBRARIES}
  GpuRayPlugin
)
target_compile_definitions(gazebo_ros_velodyne_gpu_laser PRIVATE GAZEBO_GPU_RAY=1)

install(TARGETS gazebo_ros_velodyne_laser gazebo_ros_velodyne_gpu_laser
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

add_executable(SubscribePointCloud2Message src/SubscribePointCloud2Message.cpp)
target_link_libraries(SubscribePointCloud2Message ${catkin_LIBRARIES})
add_dependencies(SubscribePointCloud2Message velodyne_gazebo_plugins_generate_messages_cpp)



