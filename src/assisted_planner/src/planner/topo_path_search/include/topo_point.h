//
// Created by Botao on 2021/8/5.
//

#ifndef SRC_TOPO_POINT_H
#define SRC_TOPO_POINT_H

#include <cluster.h>
#include <opencv2/opencv.hpp>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <plan_env/grid_map.h>
#include <plan_env/raycast.h>
#include <random>

#include "geometry_msgs/PointStamped.h"
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

using namespace std;
using namespace cv;
using namespace Eigen;

struct clusterObj {
  vector<Point> points_obj;
  Rect bounding_box;
  pcl::PointXYZ centroid;
};

class TopologyPathSearch {
private:
  /* ROS utilities */
  ros::Subscriber odom_sub, gaze_sub, depth_sub;
  ros::Publisher points_pub_vis, gaze_pub_vis, map_pub, gaze_pub, topo_points_pub;
  void OdometryCallback(const nav_msgs::Odometry::ConstPtr &odom_msg);
  void GazeCallback(const geometry_msgs::PointStamped &gaze_msg);
  void DepthCallback(const sensor_msgs::ImageConstPtr &msg);

  //vars about camera and orientation transformation
  Eigen::Isometry3d body2world = Eigen::Isometry3d::Identity();
  Eigen::Isometry3d world2body = Eigen::Isometry3d::Identity();
  Eigen::Isometry3d cam2body = Eigen::Isometry3d::Identity();
  Eigen::Matrix4f depth_cam2body;

  float cam_fx, cam_fy, cam_cx, cam_cy;
  Eigen::Matrix3f K;// intrinstic matrix
  Eigen::Matrix3f K_inverse;
  Point gaze_img;
  geometry_msgs::PointStamped gaze_src;
  Eigen::Vector3d gaze_body, gaze_world;
  nav_msgs::Odometry odom_;
  Eigen::Matrix4f odom_matrix;
  Eigen::Vector3d pos;
  double desired_speed = 0.0;

  //    //depth filter
  //    double filter_mindist, filter_maxdist, filter_scale;
  //    Mat src_filtered;
  Mat rmv_ground, rmv_ground_mask;
  Mat depth_img, depth_viz;

  //    //BFS
  Mat visited, clustered;//visited for BFS, clustered for DBSCAN
  vector<clusterObj> clusters_certain;
  vector<clusterObj> cluster_debug;


  /* data */
  GridMap::Ptr grid_map_;

  // parameter
  double max_sample_time_;
  int max_sample_num_;
  int max_raw_path_, max_raw_path2_;
  int short_cut_num_;
  Eigen::Vector3d sample_inflate_;
  double resolution_;

  //output
  Eigen::Vector3d endpoint;
  bool is_endpt_certain = false;
  vector<Eigen::Vector3d> path;
  vector<Eigen::Vector3d> waypoints;

public:
  double clearance_;
  double dis_factor = 1000.0;

  TopologyPathSearch(/* args */);
  ~TopologyPathSearch();

  void init(ros::NodeHandle &nh);
  void imgPreprocess(Mat &src);
  void removeGround(Mat &src);

  void BFS();
  void updateBody2World();
  void updateGaze();
  void updateEndpoint();
  void project2gaze();
  clusterObj DBSCAN(Point origin_pt);
  void visualization();
};

#endif//SRC_TOPO_POINT_H
