#include <algorithm>
#include <topo_point.h>
#include <eigen3/Eigen/Dense>
// #include <eigen3/Eigen/Core>
// #include <eigen3/Eigen/Geometry>
// #include <eigen3/unsupported/Eigen/MatrixFunctions>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "ros/ros.h"
#include "geometry_msgs/PointStamped.h"


using namespace std;
// using namespace cv;


int main (int argc, char** argv) {
    ros::init(argc,argv,"eye_tracker_node");
    ros::NodeHandle nh("~");

    TopologyPathSearch topo;
    topo.init(nh);
    ros::spin();
    return 1;
}