#ifndef _REBO_REPLAN_FSM_H_
#define _REBO_REPLAN_FSM_H_

#include <Eigen/Eigen>
#include <algorithm>
#include <iostream>
#include <nav_msgs/Path.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PointStamped.h>
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <vector>
#include <visualization_msgs/Marker.h>

#include <optimizer/poly_traj_optimizer.h>
#include <plan_env/grid_map.h>
#include <geometry_msgs/PoseStamped.h>
#include <quadrotor_msgs/GoalSet.h>
#include <traj_utils/DataDisp.h>
#include <plan_manage/planner_manager.h>
#include <traj_utils/planning_visualization.h>
#include <traj_utils/PolyTraj.h>
#include <traj_utils/MINCOTraj.h>

//pcl
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

using std::vector;

namespace ego_planner
{

class EGOReplanFSM
{
public:
  EGOReplanFSM() {}
  ~EGOReplanFSM() {}

  void init(ros::NodeHandle &nh);

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
  /* ---------- flag ---------- */
  enum FSM_EXEC_STATE
  {
    INIT,
    WAIT_TARGET,
    GEN_NEW_TRAJ,
    REPLAN_TRAJ,
    EXEC_TRAJ,
    EMERGENCY_STOP,
    SEQUENTIAL_START,
    RC_IN_DEAD_ZONE
  };
  enum TARGET_TYPE
  {
    MANUAL_TARGET = 1,
    PRESET_TARGET = 2,
    REFENCE_PATH = 3
  };

  /* planning utils */
  EGOPlannerManager::Ptr planner_manager_;
  PlanningVisualization::Ptr visualization_;
  traj_utils::DataDisp data_disp_;

  /* parameters */
  bool static_debug_;
  int target_type_; // 1 mannual select, 2 hard code
  double no_replan_thresh_, replan_thresh_;
  double waypoints_[50][3];
  int waypoint_num_, wpt_id_;
  double planning_horizen_;
  double emergency_time_;
  bool flag_realworld_experiment_;
  bool enable_fail_safe_;
  bool flag_escape_emergency_;
  bool rc_stop_;
  double goal_horizon_length_;

  bool have_trigger_, have_target_, have_odom_, have_new_target_, have_recv_pre_agent_, touch_goal_, mandatory_stop_;
  FSM_EXEC_STATE exec_state_;
  int continously_called_times_{0};

  Eigen::Vector3d start_pt_, start_vel_, start_acc_;   // start state
  Eigen::Vector3d end_pt_;                             // goal state
  Eigen::Vector3d local_target_pt_, local_target_vel_; // local target state
  Eigen::Vector3d odom_pos_, odom_vel_, odom_acc_;     // odometry state
  std::vector<Eigen::Vector3d> wps_;

  /* ROS utils */
  ros::NodeHandle node_;
  ros::Timer exec_timer_, safety_timer_;
  ros::Subscriber waypoint_sub_, odom_sub_, trigger_sub_, broadcast_ploytraj_sub_, mandatory_stop_sub_;
  ros::Publisher poly_traj_pub_, data_disp_pub_, broadcast_ploytraj_pub_, heartbeat_pub_, eye_gaze_line_pub_;

  /* intention capture */
  ros::Subscriber intention_sub_, topo_points_sub_;
  ros::Publisher goal_pub_, topo_point_pub_;
  double odom_yaw_;
  ros::Time last_rc_update_time_;
  void intentionCallback(const geometry_msgs::PointStampedConstPtr &msg);
  void goalFSM( const Eigen::Vector3d odom, Eigen::Vector3d &goal );
  bool goalInCollision( const Eigen::Vector3d odom, Eigen::Vector3d &goal );
  bool checkCollision( Eigen::Vector3d start_p, Eigen::Vector3d vec, double check_length, double &collision_length );
  void checkGoal(Eigen::Vector3d odom_p, Eigen::Vector3d &end_p);

  /* topo */
  bool map_topo_points_get_;
  std::vector<Eigen::Vector3d> map_topo_points_;
  int last_topo_point_id_;
  bool have_topo_point_;
  void topoPointsCallback(const sensor_msgs::PointCloud2ConstPtr &msg);
  int updateNewTopoPoint( const Eigen::Vector3d &start_pt, const Eigen::Vector3d &end_pt );

  /* state machine functions */
  void execFSMCallback(const ros::TimerEvent &e);
  void changeFSMExecState(FSM_EXEC_STATE new_state, string pos_call);
  void printFSMExecState();
  std::pair<int, EGOReplanFSM::FSM_EXEC_STATE> timesOfConsecutiveStateCalls();

  /* safety */
  void checkCollisionCallback(const ros::TimerEvent &e);
  bool callEmergencyStop(Eigen::Vector3d stop_pos);

  /* local planning */
  bool callReboundReplan(bool flag_use_poly_init, bool flag_randomPolyTraj);
  bool planFromGlobalTraj(const int trial_times = 1);
  bool planFromLocalTraj(const int trial_times = 1);

  /* global trajectory */
  void waypointCallback(const quadrotor_msgs::GoalSetPtr &msg);
  // void planGlobalTrajbyGivenWps();
  void readGivenWpsAndPlan();
  bool planNextWaypoint(const Eigen::Vector3d next_wp);

  /* input-output */
  void mandatoryStopCallback(const std_msgs::Empty &msg);
  void odometryCallback(const nav_msgs::OdometryConstPtr &msg);
  void triggerCallback(const geometry_msgs::PoseStampedPtr &msg);
  void RecvBroadcastMINCOTrajCallback(const traj_utils::MINCOTrajConstPtr &msg);
  void polyTraj2ROSMsg(traj_utils::PolyTraj &poly_msg, traj_utils::MINCOTraj &MINCO_msg);
};

} // namespace ego_planner

#endif