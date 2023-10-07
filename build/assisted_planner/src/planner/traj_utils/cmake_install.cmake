# Install script for directory: /home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/planner/traj_utils

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/traj_utils/msg" TYPE FILE FILES
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/planner/traj_utils/msg/DataDisp.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/planner/traj_utils/msg/PolyTraj.msg"
    "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/planner/traj_utils/msg/MINCOTraj.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/traj_utils/cmake" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/planner/traj_utils/catkin_generated/installspace/traj_utils-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/include/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/roseus/ros/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/common-lisp/ros/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/share/gennodejs/ros/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python3" -m compileall "/home/bottle/hbt_ws/GPA_Teleoperation/devel/lib/python3/dist-packages/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3/dist-packages" TYPE DIRECTORY FILES "/home/bottle/hbt_ws/GPA_Teleoperation/devel/lib/python3/dist-packages/traj_utils")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/planner/traj_utils/catkin_generated/installspace/traj_utils.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/traj_utils/cmake" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/planner/traj_utils/catkin_generated/installspace/traj_utils-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/traj_utils/cmake" TYPE FILE FILES
    "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/planner/traj_utils/catkin_generated/installspace/traj_utilsConfig.cmake"
    "/home/bottle/hbt_ws/GPA_Teleoperation/build/assisted_planner/src/planner/traj_utils/catkin_generated/installspace/traj_utilsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/traj_utils" TYPE FILE FILES "/home/bottle/hbt_ws/GPA_Teleoperation/src/assisted_planner/src/planner/traj_utils/package.xml")
endif()

