cmake_minimum_required(VERSION 3.0.2)
project(topo_path_search)

## Compile as C++14, supported in ROS Kinetic and newer
add_compile_options(-std=c++14)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  sensor_msgs
  roscpp
  rosbag
  std_msgs
  pcl_conversions
  pcl_ros
  plan_env
#  message_filters
#  message_generation
)

find_package(OpenCV 4  REQUIRED COMPONENTS)
find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED)

#add_message_files(
#  FILES
#  RCIn.msg
#)
#generate_messages(
# DEPENDENCIES
# std_msgs
#)

catkin_package(
  #CATKIN_DEPENDS geometry_msgs nav_msgs roscpp std_msgs
  # DEPENDS EIGEN3 PCL 
  DEPENDS EIGEN3  
  INCLUDE_DIRS include
  LIBRARIES topo_path_search
  CATKIN_DEPENDS plan_env
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

 add_executable(topo_point_node
  src/topo_point_node.cpp
  src/topo_point.cpp include/cluster.h src/cluster.cpp)

 target_link_libraries(topo_point_node
   ${catkin_LIBRARIES}
   ${PCL_LIBRARIES}
 )

 