//
// Created by bottle on 2021/8/7.
//

#include <algorithm>
#include <ctime>
#include <eigen3/Eigen/Dense>
#include <topo_point.h>

#include "geometry_msgs/PointStamped.h"
#include "ros/ros.h"

using namespace std;
using namespace cv;

TopologyPathSearch::TopologyPathSearch(/* args */) {}

TopologyPathSearch::~TopologyPathSearch() {}

void TopologyPathSearch::init(ros::NodeHandle &nh) {
  // odom_sub = nh.subscribe("/vins_estimator/imu_propagate", 10, &TopologyPathSearch::OdometryCallback,
                          //    odom_sub = nh.subscribe("/vicon_imu_ekf_odom", 10, &TopologyPathSearch::OdometryCallback,
  odom_sub = nh.subscribe("/drone_0_visual_slam/odom", 10, &TopologyPathSearch::OdometryCallback,
                          this, ros::TransportHints().tcpNoDelay());
  gaze_sub = nh.subscribe("/eye_tracker/gaze", 10, &TopologyPathSearch::GazeCallback,
                          this, ros::TransportHints().tcpNoDelay());
  // depth_sub = nh.subscribe("/camera/depth/image_rect_raw", 1,
  depth_sub = nh.subscribe("/drone_0_pcl_render_node/depth", 1,
                           &TopologyPathSearch::DepthCallback, this);
  
  // remember to change the topic when using the real drone


  //init cam2body
  cam2body = Eigen::Isometry3d::Identity();
  cam2body.rotate(Eigen::AngleAxisd(0.5 * M_PI, Eigen::Vector3d::UnitY()));
  cam2body.rotate(Eigen::AngleAxisd(-0.5 * M_PI, Eigen::Vector3d::UnitZ()));
  depth_cam2body << 0.0, 0.0, 1.0, 0.0,
          -1.0, 0.0, 0.0, 0.0,
          0.0, -1.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 1.0;

  //    // init parameter
  //    nh.getParam("topo_path_search/cam_cx", cam_cx);
  //    nh.getParam("topo_path_search/cam_cy", cam_cy);
  //    nh.getParam("topo_path_search/cam_fx", cam_fx);
  //    nh.getParam("topo_path_search/cam_fy", cam_fy);

  cam_cx = 321.04638671875;
  cam_cy = 243.44969177246094;
  cam_fx = 387.229248046875;
  cam_fy = 387.229248046875;

  K << cam_fx, 0, cam_cx,
          0, cam_fy, cam_cy,
          0, 0, 1;
  K_inverse = K.inverse();

  //DEBUG BOTAO visualization
  points_pub_vis = nh.advertise<visualization_msgs::Marker>("/cluster_point", 10);
  gaze_pub_vis = nh.advertise<visualization_msgs::Marker>("/gaze_visualization", 10);
  gaze_pub = nh.advertise<geometry_msgs::PointStamped>("/intention_capture/gaze_point", 10);
  topo_points_pub = nh.advertise<sensor_msgs::PointCloud2>("/topo_points", 10);
}

void TopologyPathSearch::DepthCallback(const sensor_msgs::ImageConstPtr &msg) {
  clock_t startTime, endTime;
  startTime = clock();

  cv_bridge::CvImageConstPtr cv_ptr;
  cv_ptr = cv_bridge::toCvCopy(msg, msg->encoding);

  depth_img = cv_ptr->image;
  depth_img.convertTo(depth_img, CV_32FC1);

  depth_img = depth_img * 1000.0;

  normalize(depth_img, depth_viz, 0, 255, cv::NORM_MINMAX);
  depth_viz.convertTo(depth_viz, CV_8UC1);

  imgPreprocess(depth_img);
  BFS();

  endTime = clock();
  //    cout << "The CLUSTER time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

  normalize(depth_img, depth_img, 0, 255, cv::NORM_MINMAX);
  applyColorMap(depth_viz, depth_viz, cv::COLORMAP_JET);
  depth_img.convertTo(depth_img, CV_8UC1);
  applyColorMap(depth_img, depth_img, cv::COLORMAP_JET);
  if (!clusters_certain.empty()) {
    for (auto iter = clusters_certain.begin(); iter < clusters_certain.end(); iter++) {
      rectangle(depth_img, iter->bounding_box, cv::Scalar(0, 255, 0), 3, cv::LINE_8, 0);
    }
           int cnt = 0;
           for (auto iter = cluster_debug.begin(); iter < cluster_debug.end(); iter++) {
               cnt++;
               rectangle(depth_img, iter->bounding_box, cv::Scalar(0, 0, 255), 3, cv::LINE_8, 0);
               putText(depth_img, to_string(cnt),iter->bounding_box.tl(),FONT_HERSHEY_SIMPLEX,2,Scalar(255,255,255),4,8,false);
           }
           circle(depth_img, gaze_img, 5,Scalar(255,255,0),-1);

  }
  visualization();

  
  //    static int cnt_imw = 0;
  //    imwrite(to_string(cnt_imw)+".png", depth_img);
  //    cnt_imw++;

    //  imshow("depth", depth_viz);
    //  waitKey(1);
  imshow("depth_raw", depth_img);
  waitKey(1);
  // imshow("clustered", clustered);
  // waitKey(1);
}

void TopologyPathSearch::OdometryCallback(const nav_msgs::Odometry::ConstPtr &odom_msg) {
  odom_ = *odom_msg;

  pos[0] = odom_.pose.pose.position.x;
  pos[1] = odom_.pose.pose.position.y;
  pos[2] = odom_.pose.pose.position.z;
  updateBody2World();

  Eigen::Quaternionf q = Eigen::Quaternionf(odom_.pose.pose.orientation.w,
                                            odom_.pose.pose.orientation.x,
                                            odom_.pose.pose.orientation.y,
                                            odom_.pose.pose.orientation.z);
  Eigen::Matrix3f rotMat = q.toRotationMatrix();

  odom_matrix.block<3, 3>(0, 0) = rotMat;
  odom_matrix(0, 3) = odom_.pose.pose.position.x;
  odom_matrix(1, 3) = odom_.pose.pose.position.y;
  odom_matrix(2, 3) = odom_.pose.pose.position.z;
  odom_matrix(3, 3) = 1.0;

  odom_matrix = odom_matrix * depth_cam2body;
  // cout<<odom_matrix<<endl;
}

void TopologyPathSearch::GazeCallback(const geometry_msgs::PointStamped &gaze_msg) {
  gaze_src = gaze_msg;
  desired_speed = gaze_msg.point.z;
  updateGaze();
}

void TopologyPathSearch::updateBody2World() {
  Eigen::Quaterniond q;
  q.x() = odom_.pose.pose.orientation.x;
  q.y() = odom_.pose.pose.orientation.y;
  q.z() = odom_.pose.pose.orientation.z;
  q.w() = odom_.pose.pose.orientation.w;
  body2world = Eigen::Isometry3d::Identity();
  body2world.rotate(q);
  world2body = body2world.inverse();
}

void TopologyPathSearch::updateGaze() {
  gaze_img.x = gaze_src.point.x;
  gaze_img.y = gaze_src.point.y;

  gaze_body[0] = double(gaze_src.point.x);
  gaze_body[1] = double(gaze_src.point.y);
  gaze_body[2] = double(1.0);
  gaze_body = K_inverse.cast<double>() * gaze_body;
  gaze_body.normalize();
  gaze_body = cam2body * gaze_body;
  gaze_world = body2world * gaze_body;

  geometry_msgs::PointStamped gaze_temp;
  gaze_temp.header.frame_id = "world";
  gaze_temp.point.x = gaze_world[0] * desired_speed;
  gaze_temp.point.y = gaze_world[1] * desired_speed;
  gaze_temp.point.z = gaze_world[2] * desired_speed;
  gaze_pub.publish(gaze_temp);

  gaze_world.normalize();
}

void TopologyPathSearch::imgPreprocess(Mat &src) {
  //    pyrDown(src, src);
  //    pyrDown(src, src);
  visited = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC1);
  clustered = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC1);
  src.convertTo(src, CV_32FC1);

  threshold(src, src, 0.2 * dis_factor, 15.0 * dis_factor, CV_THRESH_TOZERO);
  threshold(src, src, 4.5 * dis_factor, 15.0 * dis_factor, CV_THRESH_TOZERO_INV);

  // removeGround(src);
}

/*
 * @brief: remove the ground in depth image
 */
void TopologyPathSearch::removeGround(Mat &src) {
  for (int i = 0; i < src.cols; i++) {
    for (int j = 0; j < src.rows; j++) {
      float z_depth;
      if (src.at<float>(j, i) > 0.0) {
        z_depth = src.at<float>(j, i) / dis_factor;

        Eigen::Vector3f proj_pt;
        proj_pt(0) = (float(i) - cam_cx) * z_depth / cam_fx;
        proj_pt(1) = (float(j) - cam_cy) * z_depth / cam_fy;
        proj_pt(2) = z_depth;

        proj_pt = odom_matrix.block<3, 3>(0, 0) * proj_pt + pos.cast<float>();

        // cout<<proj_pt[2]<<endl;

        if (proj_pt[2] < 0.2 || proj_pt[2] > 2.5) {
          src.at<float>(j, i) = 0.0;
        }
      }
    }
  }
}

clusterObj TopologyPathSearch::DBSCAN(Point origin_pt) {
  vector<Point> unvisited_temp;
  clusterObj cluster_temp;
  cluster_temp.points_obj.push_back(origin_pt);
//  cluster_temp.ori_point = origin_pt;
  unvisited_temp.push_back(origin_pt);

  while (!unvisited_temp.empty()) {
    int cnt = 0;
    Point cur_pt = unvisited_temp.back();
    unvisited_temp.pop_back();
    clustered.at<uchar>(cur_pt.y, cur_pt.x) = 255;

    if (cur_pt.x == (clustered.cols - 1) || cur_pt.x == 0 || cur_pt.y == 0 || cur_pt.y == (clustered.rows - 1)) continue;

    for (int i = cur_pt.x - 1; i <= cur_pt.x + 1; i++) {
      for (int j = cur_pt.y - 1; j <= cur_pt.y + 1; j++) {
        if (fabs(depth_img.at<float>(j, i) - depth_img.at<float>(cur_pt.y, cur_pt.x)) < 0.7 * dis_factor &&
          depth_img.at<float>(j, i) > (dis_factor / 10.0)) {
          cnt++;
          if (!clustered.at<uchar>(j, i)) {
            Point pt_temp;
            pt_temp.x = i;
            pt_temp.y = j;
            unvisited_temp.push_back(pt_temp);
            clustered.at<uchar>(j, i) = 255;
          }
        }
      }
    }

    if (cnt > 3) cluster_temp.points_obj.push_back(cur_pt);
  }

  cluster_temp.bounding_box = boundingRect(cluster_temp.points_obj);
  float avg_depth = 0.0;
  for (auto iter = cluster_temp.points_obj.begin(); iter < cluster_temp.points_obj.end(); iter++) {
    avg_depth += depth_img.at<float>(iter->y, iter->x);
  }
  avg_depth = avg_depth / float(cluster_temp.points_obj.size());
  Eigen::Vector3f centroid;
  centroid[0] = float(cluster_temp.bounding_box.x + cluster_temp.bounding_box.width / 2.0);
  centroid[1] = float(cluster_temp.bounding_box.y + cluster_temp.bounding_box.height / 2.0);
  centroid[2] = 1;
  centroid = (avg_depth / dis_factor) * K_inverse * centroid;

  centroid = cam2body.cast<float>() * centroid;
  cluster_temp.centroid.x = centroid[0];
  cluster_temp.centroid.y = centroid[1];
  cluster_temp.centroid.z = centroid[2];

  return cluster_temp;
}

/*
 * @brief: Breadth-First Search
 */
void TopologyPathSearch::BFS() {
  //TODO: gaze_img
  queue<Point> unvisited_bfs;
  clusters_certain.clear();
  clusterObj clusters_uncertain;

  //DEBUG
  cluster_debug.clear();

  unvisited_bfs.push(gaze_img);

  while (!unvisited_bfs.empty()) {
    Point cur_pt = unvisited_bfs.front();
    unvisited_bfs.pop();
    visited.at<uchar>(cur_pt.y, cur_pt.x) = 255;

    if (depth_img.at<float>(cur_pt.y, cur_pt.x) >= (dis_factor / 10.0) && !clustered.at<uchar>(cur_pt.y, cur_pt.x)) {
      clusterObj cluster_temp = DBSCAN(cur_pt);

      // cout<<cluster_temp.points_obj.size()<<endl;

      if (cluster_temp.points_obj.size() > 500) {
        if (cluster_temp.bounding_box.contains(gaze_img)) {
          clusters_certain.push_back(cluster_temp);

        } else {
          if (clusters_uncertain.points_obj.size() > 1) {
            vector<Point> merge;
            Rect merge_bdbox;
            merge = clusters_uncertain.points_obj;
            merge.insert(merge.end(), cluster_temp.points_obj.begin(), cluster_temp.points_obj.end());
            merge_bdbox = boundingRect(merge);
            if (merge_bdbox.contains(gaze_img)) {
              clusters_certain.push_back(cluster_temp);
              clusters_certain.push_back(clusters_uncertain);
              return;
            }
          } else {
            clusters_uncertain = cluster_temp;
          }
        }
      }
    }

    if (cur_pt.x == (visited.cols - 1) || cur_pt.x == 0 || cur_pt.y == 0 || cur_pt.y == (visited.rows - 1))
      continue;

    for (int i = cur_pt.x - 1; i <= cur_pt.x + 1; i++) {
      for (int j = cur_pt.y - 1; j <= cur_pt.y + 1; j++) {
        if (!visited.at<uchar>(j, i)) {
          Point pt_temp;
          pt_temp.x = i;
          pt_temp.y = j;
          unvisited_bfs.push(pt_temp);
          visited.at<uchar>(j, i) = 255;
        }
      }
    }
  }
}

/*
 * @brief: Project centroid of all clustered obstacles to the gaze path
 */
void TopologyPathSearch::project2gaze() {
}

void TopologyPathSearch::visualization() {
  geometry_msgs::PointStamped point_temp;
  pcl::PointCloud<pcl::PointXYZ> topo_points;
  pcl::PointXYZ topo_temp;

  visualization_msgs::Marker points, line_strip;
  points.type = visualization_msgs::Marker::POINTS;
  points.scale.x = 0.2;
  points.scale.y = 0.2;
  points.color.r = 1.0f;
  points.color.a = 1.0;
  points.header.frame_id = "world";
  line_strip.id = 1;
  line_strip.type = visualization_msgs::Marker::LINE_STRIP;
  line_strip.scale.x = 0.1;
  line_strip.color.g = 1.0;
  line_strip.color.a = 1.0;
  line_strip.header.frame_id = "world";
  point_temp.point.x = pos[0];
  point_temp.point.y = pos[1];
  point_temp.point.z = pos[2];
  // points.points.push_back(point_temp.point);
  line_strip.points.push_back(point_temp.point);

  for (auto iter = clusters_certain.begin(); iter < clusters_certain.end(); iter++) {
    Eigen::Vector3d centroid_world;
    centroid_world[0] = iter->centroid.x;
    centroid_world[1] = iter->centroid.y;
    centroid_world[2] = iter->centroid.z;
    centroid_world = body2world * centroid_world + pos;

    Eigen::Vector3d prg2gaze;
    prg2gaze = gaze_world.dot(centroid_world - pos) * gaze_world + pos;
    topo_temp.x = prg2gaze[0];
    topo_temp.y = prg2gaze[1];
    topo_temp.z = prg2gaze[2];
    topo_points.push_back(topo_temp);
    point_temp.point.x = prg2gaze[0];
    point_temp.point.y = prg2gaze[1];
    point_temp.point.z = prg2gaze[2];
    points.points.push_back(point_temp.point);
  }

  endpoint = pos + gaze_world * 5;
  point_temp.point.x = endpoint[0];
  point_temp.point.y = endpoint[1];
  point_temp.point.z = endpoint[2];
  line_strip.points.push_back(point_temp.point);

  pcl::PCLPointCloud2 pcl_pc;
  sensor_msgs::PointCloud2 msg_pub;
  pcl::toPCLPointCloud2(topo_points, pcl_pc);
  pcl_conversions::fromPCL(pcl_pc, msg_pub);
  msg_pub.header.frame_id = "world";
  topo_points_pub.publish(msg_pub);

  points_pub_vis.publish(points);
  gaze_pub_vis.publish(line_strip);
}
