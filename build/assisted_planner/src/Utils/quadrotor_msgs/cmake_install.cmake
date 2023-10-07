# Install script for directory: /home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/bottle/hbt_ws/GPA_Teleoperation/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/quadrotor_msgs/msg" TYPE FILE FILES
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/AuxCommand.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/Corrections.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/Gains.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/OutputData.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/PositionCommand.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/PPROutputData.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/Serial.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/SO3Command.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/StatusData.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/TRPYCommand.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/Odometry.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/PolynomialTrajectory.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/LQRTrajectory.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/msg/GoalSet.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/quadrotor_msgs/cmake" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/Utils/quadrotor_msgs/catkin_generated/installspace/quadrotor_msgs-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/include/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/roseus/ros/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/common-lisp/ros/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/gennodejs/ros/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python3" -m compileall "/home/bottle/hbt_ws/GPA_Teleoperation/devel/lib/python3/dist-packages/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3/dist-packages" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/lib/python3/dist-packages/quadrotor_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/Utils/quadrotor_msgs/catkin_generated/installspace/quadrotor_msgs.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/quadrotor_msgs/cmake" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/Utils/quadrotor_msgs/catkin_generated/installspace/quadrotor_msgs-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/quadrotor_msgs/cmake" TYPE FILE FILES
    "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/Utils/quadrotor_msgs/catkin_generated/installspace/quadrotor_msgsConfig.cmake"
    "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/Utils/quadrotor_msgs/catkin_generated/installspace/quadrotor_msgsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/quadrotor_msgs" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/Utils/quadrotor_msgs/package.xml")
endif()

