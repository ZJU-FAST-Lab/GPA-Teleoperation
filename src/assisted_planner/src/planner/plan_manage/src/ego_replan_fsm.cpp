
#include <plan_manage/ego_replan_fsm.h>

namespace ego_planner
{

  void EGOReplanFSM::init(ros::NodeHandle &nh)
  {
    exec_state_ = FSM_EXEC_STATE::INIT;
    have_target_ = false;
    have_odom_ = false;
    have_recv_pre_agent_ = false;
    flag_escape_emergency_ = true;
    mandatory_stop_ = false;
    rc_stop_ = false;
    map_topo_points_get_ = false;
    have_topo_point_ = false;
    last_rc_update_time_.fromSec(0);

    /*  fsm param  */
    nh.param("fsm/static_debug", static_debug_, false);
    nh.param("fsm/flight_type", target_type_, -1);
    nh.param("fsm/thresh_replan_time", replan_thresh_, -1.0);
    nh.param("fsm/thresh_no_replan_meter", no_replan_thresh_, -1.0);
    nh.param("fsm/planning_horizon", planning_horizen_, -1.0);
    nh.param("fsm/emergency_time", emergency_time_, 1.0);
    nh.param("fsm/realworld_experiment", flag_realworld_experiment_, false);
    nh.param("fsm/fail_safe", enable_fail_safe_, true);

    nh.param("fsm/goal_horizon_length", goal_horizon_length_, 5.0);

    have_trigger_ = !flag_realworld_experiment_;

    nh.param("fsm/waypoint_num", waypoint_num_, -1);
    for (int i = 0; i < waypoint_num_; i++)
    {
      nh.param("fsm/waypoint" + to_string(i) + "_x", waypoints_[i][0], -1.0);
      nh.param("fsm/waypoint" + to_string(i) + "_y", waypoints_[i][1], -1.0);
      nh.param("fsm/waypoint" + to_string(i) + "_z", waypoints_[i][2], -1.0);
    }

    /* initialize main modules */
    visualization_.reset(new PlanningVisualization(nh));
    planner_manager_.reset(new EGOPlannerManager);
    planner_manager_->initPlanModules(nh, visualization_);

    /* callback */
    exec_timer_ = nh.createTimer(ros::Duration(0.01), &EGOReplanFSM::execFSMCallback, this);
    safety_timer_ = nh.createTimer(ros::Duration(0.05), &EGOReplanFSM::checkCollisionCallback, this);

    odom_sub_ = nh.subscribe("odom_world", 1, &EGOReplanFSM::odometryCallback, this);
    mandatory_stop_sub_ = nh.subscribe("mandatory_stop", 1, &EGOReplanFSM::mandatoryStopCallback, this);

    /* Use MINCO trajectory to minimize the message size in wireless communication */
    broadcast_ploytraj_pub_ = nh.advertise<traj_utils::MINCOTraj>("planning/broadcast_traj_send", 10);
    broadcast_ploytraj_sub_ = nh.subscribe<traj_utils::MINCOTraj>("planning/broadcast_traj_recv", 100,
                                                                  &EGOReplanFSM::RecvBroadcastMINCOTrajCallback,
                                                                  this,
                                                                  ros::TransportHints().tcpNoDelay());

    poly_traj_pub_ = nh.advertise<traj_utils::PolyTraj>("planning/trajectory", 10);
    data_disp_pub_ = nh.advertise<traj_utils::DataDisp>("planning/data_display", 100);
    heartbeat_pub_ = nh.advertise<std_msgs::Empty>("planning/heartbeat", 10);

    if (target_type_ == TARGET_TYPE::MANUAL_TARGET)
    {
      waypoint_sub_ = nh.subscribe("/goal", 1, &EGOReplanFSM::waypointCallback, this);
    }
    else if (target_type_ == TARGET_TYPE::PRESET_TARGET)
    {
      trigger_sub_ = nh.subscribe("/traj_start_trigger", 1, &EGOReplanFSM::triggerCallback, this);

      ROS_INFO("Wait for 2 second.");
      int count = 0;
      while (ros::ok() && count++ < 2000)
      {
        ros::spinOnce();
        ros::Duration(0.001).sleep();
      }

      // ROS_INFO("Waiting for trigger from RC");

      // while (ros::ok() && (!have_odom_ || !have_trigger_))
      // {
      //   ros::spinOnce();
      //   ros::Duration(0.001).sleep();
      // }

      readGivenWpsAndPlan();
    }
    else
      cout << "Wrong target_type_ value! target_type_=" << target_type_ << endl;

    // ! topo point
    topo_points_sub_ = nh.subscribe("/topo_points", 1, &EGOReplanFSM::topoPointsCallback, this);
    topo_point_pub_ = nh.advertise<sensor_msgs::PointCloud2>("/topo_point", 10);
    // ! intention capture
    intention_sub_ = nh.subscribe("/intention_capture/gaze_point", 1, &EGOReplanFSM::intentionCallback, this);
    goal_pub_ = nh.advertise<quadrotor_msgs::GoalSet>("/goal_with_id", 10);
    eye_gaze_line_pub_ = nh.advertise<visualization_msgs::MarkerArray>("eye_gaze_line", 2);
  }

  void EGOReplanFSM::topoPointsCallback(const sensor_msgs::PointCloud2ConstPtr &msg)
  {
    if( map_topo_points_get_ )
      return;

    // ROS_ERROR_STREAM("TOPO POINT RECEIVED");
    // map_topo_points_get_ = true;
    pcl::PointCloud<pcl::PointXYZ> points_in;
    pcl::fromROSMsg(*msg, points_in);

    map_topo_points_.clear();
    for( int i = 0; i < points_in.points.size(); i++ )
    {
      Eigen::Vector3d temp_point{ points_in.points[i].x, points_in.points[i].y, points_in.points[i].z };
      map_topo_points_.push_back(temp_point);
    }
  }

  void EGOReplanFSM::intentionCallback(const geometry_msgs::PointStampedConstPtr &msg)
  {
    last_rc_update_time_ = ros::Time::now();

    if (msg->header.frame_id == "no_goal")
    {
      // 防止开机就转头
      if( exec_state_ == WAIT_TARGET )
        return;
      if(!rc_stop_)
      {
        rc_stop_ = true;
        changeFSMExecState(RC_IN_DEAD_ZONE, "RC");
      }
      return;
    }

    rc_stop_ = false;

    Eigen::Vector3d desire_goal;
    desire_goal[0] = msg->point.x;
    desire_goal[1] = msg->point.y;
    desire_goal[2] = msg->point.z;
    double desire_vel = desire_goal.norm();
    double length;
    if( desire_vel > 1.0 )
    {
      length = goal_horizon_length_;
    }
    else
    {
      length = goal_horizon_length_ * desire_vel;
    }
    //TODO 更新一下速度
    planner_manager_->updatePlanParam( desire_vel );
    planner_manager_->updateYaw(odom_yaw_);

    double yaw = odom_yaw_;
    Eigen::Vector3d pos = odom_pos_;

    desire_goal.normalize();
    desire_goal = desire_goal * length;
    Eigen::Vector3d body_goal{0, 0, 0};
    body_goal[0] = cos(yaw) * desire_goal[0] + cos(yaw + M_PI/2) * desire_goal[1];
    body_goal[1] = sin(yaw) * desire_goal[0] + sin(yaw + M_PI/2) * desire_goal[1];
    body_goal[2] = desire_goal[2];
    pos = pos + body_goal;

    // 可视化一下
    std::vector<Eigen::Vector3d> eye_gaze_line;
    eye_gaze_line.clear();
    eye_gaze_line.push_back(odom_pos_);
    Eigen::Vector3d eye_gaze = body_goal;
    eye_gaze.normalize();
    eye_gaze_line.push_back(pos);
    Eigen::Vector4d color(0.5, 0, 0.8, 0.8);
    visualization_->displayArrowList (eye_gaze_line_pub_, eye_gaze_line, 0.05 , color, 1000);

    quadrotor_msgs::GoalSet goal_msg;
    goal_msg.drone_id = planner_manager_->pp_.drone_id;
    goal_msg.goal[0] = pos[0];
    goal_msg.goal[1] = pos[1];
    goal_msg.goal[2] = pos[2];
    goal_pub_.publish( goal_msg );
  }

  void EGOReplanFSM::goalFSM( const Eigen::Vector3d odom, Eigen::Vector3d &goal )
  {
    // 检查射线上是否有障碍物
    // 如果有障碍物检查一下是否可以轻松翻越
    // 如果不能轻松翻越就把goal给在前面
    if( goalInCollision( odom, goal ) )
    {
      // 应该做一些更多的操作
    }
  }

  bool EGOReplanFSM::goalInCollision( const Eigen::Vector3d odom, Eigen::Vector3d &goal )
  {
    Eigen::Vector3d temp_vec = goal - odom;
    double length = temp_vec.norm();
    temp_vec.normalize();
    temp_vec = temp_vec * 0.1;
    Eigen::Vector3d temp_point = odom;
    double i = 0;

    for( ; i < length; i += 0.1 )
    {
      temp_point = temp_point + temp_vec;
      if( planner_manager_->grid_map_->getOccupancy(temp_point) != 0 )
        break;
    }
    goal = temp_point - temp_vec * 5;

    if( i > length )
      return false;
    
    return true;
  }

  void EGOReplanFSM::execFSMCallback(const ros::TimerEvent &e)
  {
    exec_timer_.stop(); // To avoid blockage
    std_msgs::Empty heartbeat_msg;
    heartbeat_pub_.publish(heartbeat_msg);

    static int fsm_num = 0;
    fsm_num++;
    if (fsm_num == 500)
    {
      fsm_num = 0;
      printFSMExecState();
    }

    // if( (ros::Time::now() - last_rc_update_time_).toSec() > 0.25 )
    // {
    //   if(!rc_stop_)
    //   {
    //     rc_stop_ = true;
    //     changeFSMExecState(RC_IN_DEAD_ZONE, "RC");
    //   }
    // }

    switch (exec_state_)
    {
    case INIT:
    {
      if (!have_odom_)
      {
        goto force_return; // return;
      }
      changeFSMExecState(WAIT_TARGET, "FSM");
      break;
    }

    case WAIT_TARGET:
    {
      if (!have_target_ || !have_trigger_)
        goto force_return; // return;
      else
      {
        changeFSMExecState(SEQUENTIAL_START, "FSM");
      }
      break;
    }

    case SEQUENTIAL_START: // for swarm or single drone with drone_id = 0
    {
      if (planner_manager_->pp_.drone_id <= 0 || (planner_manager_->pp_.drone_id >= 1 && have_recv_pre_agent_))
      {
        bool success = planFromGlobalTraj(10); // zx-todo
        if (success)
        {
          changeFSMExecState(EXEC_TRAJ, "FSM");
        }
        else
        {
          ROS_WARN("Failed to generate the first trajectory, keep trying");
          changeFSMExecState(SEQUENTIAL_START, "FSM"); // "changeFSMExecState" must be called each time planned
        }
      }

      break;
    }

    case GEN_NEW_TRAJ:
    {

      bool success = planFromGlobalTraj(10); // zx-todo
      if (success)
      {
        changeFSMExecState(EXEC_TRAJ, "FSM");
        flag_escape_emergency_ = true;
      }
      else
      {
        changeFSMExecState(GEN_NEW_TRAJ, "FSM"); // "changeFSMExecState" must be called each time planned
      }
      break;
    }

    case REPLAN_TRAJ:
    {

      if (planFromLocalTraj(1))
      {
        changeFSMExecState(EXEC_TRAJ, "FSM");
      }
      else
      {
        changeFSMExecState(REPLAN_TRAJ, "FSM");
      }

      break;
    }

    case EXEC_TRAJ:
    {
      /* determine if need to replan */
      LocalTrajData *info = &planner_manager_->traj_.local_traj;
      double t_cur = ros::Time::now().toSec() - info->start_time;
      t_cur = min(info->duration, t_cur);

      Eigen::Vector3d pos = info->traj.getPos(t_cur);
      bool touch_the_goal = ((local_target_pt_ - end_pt_).norm() < 1e-2);

      if ((target_type_ == TARGET_TYPE::PRESET_TARGET) &&
          (wpt_id_ < waypoint_num_ - 1) &&
          (end_pt_ - pos).norm() < no_replan_thresh_)
      {
        wpt_id_++;
        planNextWaypoint(wps_[wpt_id_]);
      }
      else if ((t_cur > info->duration - 1e-2) && touch_the_goal) // local target close to the global target
      {
        have_target_ = false;
        have_trigger_ = false;

        if (target_type_ == TARGET_TYPE::PRESET_TARGET)
        {
          wpt_id_ = 0;
          planNextWaypoint(wps_[wpt_id_]);
        }

        /* The navigation task completed */
        changeFSMExecState(WAIT_TARGET, "FSM");
        goto force_return;
      }
      else if ((end_pt_ - pos).norm() < no_replan_thresh_)
      {
        if (planner_manager_->grid_map_->getInflateOccupancy(end_pt_))
        {
          have_target_ = false;
          have_trigger_ = false;
          ROS_ERROR("The goal is in obstacles, finish the planning.");
          callEmergencyStop(odom_pos_);

          /* The navigation task completed */
          changeFSMExecState(WAIT_TARGET, "FSM");
          goto force_return;
        }
        else
        {
          // pass;
        }
      }
      else if (t_cur > replan_thresh_ ||
               (!touch_the_goal && planner_manager_->traj_.local_traj.pts_chk.back().back().first - t_cur < emergency_time_))
      {
        changeFSMExecState(REPLAN_TRAJ, "FSM");
      }

      break;
    }

    case EMERGENCY_STOP:
    {
      if (flag_escape_emergency_) // Avoiding repeated calls
      {
        callEmergencyStop(odom_pos_);
      }
      else
      {
        if (enable_fail_safe_ && odom_vel_.norm() < 0.1)
          changeFSMExecState(GEN_NEW_TRAJ, "FSM");
      }
      flag_escape_emergency_ = false;
      break;
    }

    case RC_IN_DEAD_ZONE:
    {
      if(flag_escape_emergency_)
      {
        callEmergencyStop(odom_pos_);
      }
      else
      {
        if (rc_stop_)
          break;
        else
        {
          changeFSMExecState(GEN_NEW_TRAJ, "FSM");
        }
      }
      
      flag_escape_emergency_ = false;
      break;
    }
    }

    data_disp_.header.stamp = ros::Time::now();
    data_disp_pub_.publish(data_disp_);

  force_return:;
    exec_timer_.start();
  }

  void EGOReplanFSM::changeFSMExecState(FSM_EXEC_STATE new_state, string pos_call)
  {

    if (new_state == exec_state_)
      continously_called_times_++;
    else
      continously_called_times_ = 1;

    static string state_str[8] = {"INIT", "WAIT_TARGET", "GEN_NEW_TRAJ", "REPLAN_TRAJ", "EXEC_TRAJ", "EMERGENCY_STOP", "SEQUENTIAL_START", "RC_IN_DEAD_ZONE"};
    int pre_s = int(exec_state_);
    exec_state_ = new_state;
    cout << "[" + pos_call + "]: from " + state_str[pre_s] + " to " + state_str[int(new_state)] << endl;
  }

  void EGOReplanFSM::printFSMExecState()
  {
    static string state_str[8] = {"INIT", "WAIT_TARGET", "GEN_NEW_TRAJ", "REPLAN_TRAJ", "EXEC_TRAJ", "EMERGENCY_STOP", "SEQUENTIAL_START", "RC_IN_DEAD_ZONE"};
    // static int last_printed_state = -1, dot_nums = 0;

    // if (exec_state_ != last_printed_state)
    //   dot_nums = 0;
    // else
    //   dot_nums++;

    cout << "\r[FSM]: state: " + state_str[int(exec_state_)];

    // last_printed_state = exec_state_;

    // some warnings
    if (!have_odom_)
    {
      cout << ", waiting for odom";
    }
    if (!have_target_)
    {
      cout << ", waiting for target";
    }
    if (!have_trigger_)
    {
      cout << ", waiting for trigger";
    }
    if (planner_manager_->pp_.drone_id >= 1 && !have_recv_pre_agent_)
    {
      cout << ", haven't receive traj from previous drone";
    }

    cout << endl;

    // cout << string(dot_nums, '.');

    // fflush(stdout);
  }

  std::pair<int, EGOReplanFSM::FSM_EXEC_STATE> EGOReplanFSM::timesOfConsecutiveStateCalls()
  {
    return std::pair<int, FSM_EXEC_STATE>(continously_called_times_, exec_state_);
  }

  void EGOReplanFSM::checkCollisionCallback(const ros::TimerEvent &e)
  {

    LocalTrajData *info = &planner_manager_->traj_.local_traj;
    auto map = planner_manager_->grid_map_;
    double t_cur = ros::Time::now().toSec() - info->start_time;
    PtsChk_t pts_chk = info->pts_chk;

    if (exec_state_ == WAIT_TARGET || info->traj_id <= 0)
      return;

    /* ---------- check lost of depth ---------- */
    if (map->getOdomDepthTimeout())
    {
      ROS_ERROR("Depth Lost! EMERGENCY_STOP");
      enable_fail_safe_ = false;
      changeFSMExecState(EMERGENCY_STOP, "SAFETY");
    }

    // bool close_to_the_end_of_safe_segment = (pts_chk.back().back().first - t_cur) < emergency_time_;
    // // bool close_to_goal = (info->traj.getPos(info->duration) - end_pt_).norm() < 1e-5;
    // if (close_to_the_end_of_safe_segment)
    // {
    //   changeFSMExecState(REPLAN_TRAJ, "SAFETY");
    //   return;

    //   // if (!close_to_goal)
    //   // {
    //   //   // ROS_INFO("current position is close to the safe segment end.");
    //   //   changeFSMExecState(REPLAN_TRAJ, "SAFETY");
    //   //   return;
    //   // }
    //   // else
    //   // {
    //   //   double t_step = map->getResolution() / planner_manager_->pp_.max_vel_;
    //   //   for (double t = pts_chk.back().back().first; t < info->duration; t += t_step)
    //   //   {
    //   //     if (map->getInflateOccupancy(info->traj.getPos(t)))
    //   //     {
    //   //       if ((odom_pos_ - end_pt_).norm() < no_replan_thresh_)
    //   //       {
    //   //         ROS_ERROR("Dense obstacles close to the goal, stop planning.");
    //   //         callEmergencyStop(odom_pos_);
    //   //         have_target_ = false;
    //   //         changeFSMExecState(WAIT_TARGET, "SAFETY");
    //   //         return;
    //   //       }
    //   //       else
    //   //       {
    //   //         changeFSMExecState(REPLAN_TRAJ, "SAFETY");
    //   //         return;
    //   //       }
    //   //     }
    //   //   }
    //   // }
    // }

    /* ---------- check trajectory ---------- */
    const double CLEARANCE = 0.8 * planner_manager_->getSwarmClearance();
    auto id_ratio = info->traj.locatePieceIdxWithRatio(t_cur);

    // cout << "t_cur=" << t_cur << " info->duration=" << info->duration << endl;

    size_t i_start = floor((id_ratio.first + id_ratio.second) * planner_manager_->getCpsNumPrePiece());
    if (i_start >= pts_chk.size())
    {
      // ROS_ERROR("i_start >= pts_chk.size()");
      return;
    }
    size_t j_start = 0;
    // cout << "i_start=" << i_start << " pts_chk.size()=" << pts_chk.size() << " pts_chk[i_start].size()=" << pts_chk[i_start].size() << endl;
    for (; i_start < pts_chk.size(); ++i_start)
    {
      for (j_start = 0; j_start < pts_chk[i_start].size(); ++j_start)
      {
        if (pts_chk[i_start][j_start].first > t_cur)
        {
          goto find_ij_start;
        }
      }
    }
  find_ij_start:;

    // Eigen::Vector3d last_pt = pts_chk[0][0].second;
    // for (size_t i = 0; i < pts_chk.size(); ++i)
    // {
    //   cout << "--------------------" << endl;
    //   for (size_t j = 0; j < pts_chk[i].size(); ++j)
    //   {
    //     cout << pts_chk[i][j].first << " @ " << pts_chk[i][j].second.transpose() << " @ " << (pts_chk[i][j].second - last_pt).transpose() << " @ " << map->getInflateOccupancy(pts_chk[i][j].second) << endl;
    //     last_pt = pts_chk[i][j].second;
    //   }
    // }

    // cout << "pts_chk[i_start][j_start].first - t_cur = " << pts_chk[i_start][j_start].first - t_cur << endl;
    // cout << "devi = " << (pts_chk[i_start][j_start].second - info->traj.getPos(t_cur)).transpose() << endl;

    // cout << "pts_chk.size()=" << pts_chk.size() << " i_start=" << i_start << endl;
    // Eigen::Vector3d p_last = pts_chk[i_start][j_start].second;
    const bool touch_the_end = ((local_target_pt_ - end_pt_).norm() < 1e-2);
    size_t i_end = touch_the_end ? pts_chk.size() : pts_chk.size() * 3 / 4;
    for (size_t i = i_start; i < i_end; ++i)
    {
      for (size_t j = j_start; j < pts_chk[i].size(); ++j)
      {

        double t = pts_chk[i][j].first;
        Eigen::Vector3d p = pts_chk[i][j].second;
        // if ( (p - p_last).cwiseAbs().maxCoeff() > planner_manager_->grid_map_->getResolution() * 1.05 )
        // {
        //   ROS_ERROR("BBBBBBBBBBBBBBBBBBBBBBBBBBB");
        //   cout << "p=" << p.transpose() << " p_last=" << p_last.transpose() << " dist=" << (p - p_last).cwiseAbs().maxCoeff() << endl;
        // }
        // p_last = p;

        // cout << "t=" << t << " @ "
        //      << "p=" << p.transpose() << endl;
        // If t_cur < t_2_3, only the first 2/3 partition of the trajectory is considered valid and will get checked.
        // if (t_cur < t_2_3 && t >= t_2_3)
        //   break;

        bool occ = false;
        occ |= map->getInflateOccupancy(p);

        // cout << "p=" << p.transpose() << endl;

        // if (occ)
        // {
        //   ROS_WARN("AAAAAAAAAAAAAAAAAAA");
        //   cout << "pts_chk[i_start].size()=" << pts_chk[i_start].size() << endl;
        //   cout << "i=" << i << " j=" << j << " i_start=" << i_start << " j_start=" << j_start << endl;
        //   cout << "pts_chk.size()=" << pts_chk.size() << endl;
        //   cout << "t=" << t << endl;
        //   cout << "from t=" << info->traj.getPos(t).transpose() << endl;
        //   cout << "from rec=" << p.transpose() << endl;
        // }

        for (size_t id = 0; id < planner_manager_->traj_.swarm_traj.size(); id++)
        {
          if ((planner_manager_->traj_.swarm_traj.at(id).drone_id != (int)id) ||
              (planner_manager_->traj_.swarm_traj.at(id).drone_id == planner_manager_->pp_.drone_id))
          {
            continue;
          }

          double t_X = t - (info->start_time - planner_manager_->traj_.swarm_traj.at(id).start_time);
          if (t_X > 0 && t_X < planner_manager_->traj_.swarm_traj.at(id).duration)
          {
            Eigen::Vector3d swarm_pridicted = planner_manager_->traj_.swarm_traj.at(id).traj.getPos(t_X);
            double dist = (p - swarm_pridicted).norm();

            if (dist < CLEARANCE)
            {
              ROS_WARN("swarm distance between drone %d and drone %d is %f, too close!",
                       planner_manager_->pp_.drone_id, (int)id, dist);
              occ = true;
              break;
            }
          }
        }

        if (occ)
        {
          /* Handle the collided case immediately */
          if (planFromLocalTraj()) // Make a chance
          {
            ROS_INFO("Plan success when detect collision. %f", t / info->duration);
            changeFSMExecState(EXEC_TRAJ, "SAFETY");
            return;
          }
          else
          {
            if (t - t_cur < emergency_time_) // 0.8s of emergency time
            {
              ROS_WARN("Emergency stop! time=%f", t - t_cur);
              changeFSMExecState(EMERGENCY_STOP, "SAFETY");
            }
            else
            {
              ROS_WARN("current traj in collision, replan.");
              changeFSMExecState(REPLAN_TRAJ, "SAFETY");
            }
            return;
          }
          break;
        }
      }
      j_start = 0;
    }
  }

  bool EGOReplanFSM::callEmergencyStop(Eigen::Vector3d stop_pos)
  {

    planner_manager_->EmergencyStop(stop_pos);

    traj_utils::PolyTraj poly_msg;
    traj_utils::MINCOTraj MINCO_msg;
    polyTraj2ROSMsg(poly_msg, MINCO_msg);
    poly_traj_pub_.publish(poly_msg);
    broadcast_ploytraj_pub_.publish(MINCO_msg);

    return true;
  }

  int EGOReplanFSM::updateNewTopoPoint( const Eigen::Vector3d &start_pt, const Eigen::Vector3d &end_pt )
  {
    std::vector<int> alternative_topo_points;
    alternative_topo_points.clear();

    for( int i = 0; i < map_topo_points_.size(); i++ )
    {
      if( (map_topo_points_[i] - start_pt).norm() < 5.0 )
        alternative_topo_points.push_back(i);
    }
    
    if( alternative_topo_points.size() == 0 )
      return -1;

    Eigen::Vector3d vec = end_pt - start_pt;
    Eigen::Vector3d temp_point = start_pt;
    Eigen::Vector3d temp_topo_point;
    double min_length = 100000.0;
    int min_id = 0;
    double length = vec.norm();
    vec.normalize();
    double res = 0.3;
    vec = vec * res;
    for( double i = res; i < length; i += res )
    {
      temp_point = temp_point + vec;
      for( int j = 0; j < alternative_topo_points.size(); j++ )
      {
        temp_topo_point = map_topo_points_[alternative_topo_points[j]];
        double temp_length = (temp_topo_point - temp_point).norm();
        if( temp_length < min_length )
        {
          min_length = temp_length;
          min_id = alternative_topo_points[j];
        }
      }
    }

    pcl::PointCloud<pcl::PointXYZI> topo_point_pc;
    if( min_length < 4.0 &&  min_length > 0.3)
    {
      pcl::PointXYZI temp_pcl_point;
      temp_pcl_point.x = map_topo_points_[min_id][0];
      temp_pcl_point.y = map_topo_points_[min_id][1];
      temp_pcl_point.z = map_topo_points_[min_id][2];
      temp_pcl_point.intensity = 0;
      topo_point_pc.points.push_back( temp_pcl_point );
    }
    else
    {
      min_id = -1;
    }
    
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(topo_point_pc, cloud_msg);
    cloud_msg.header.frame_id = "world";
    topo_point_pub_.publish(cloud_msg);
  
    return min_id;
  }


  bool EGOReplanFSM::callReboundReplan(bool flag_use_poly_init, bool flag_randomPolyTraj)
  {

    planner_manager_->getLocalTarget(
        planning_horizen_, start_pt_, end_pt_,
        local_target_pt_, local_target_vel_,
        touch_goal_);

    // 判断一下是否可以用老的轨迹做初值
    // if( have_new_target_ )
    // {
    //   int topo_point_id;
    //   topo_point_id = updateNewTopoPoint( start_pt_, local_target_pt_ );
    //   if( topo_point_id >= 0 )
    //   {
    //     if( !have_topo_point_ )
    //     {
    //       have_topo_point_ = true;
    //       last_topo_point_id_ = topo_point_id;
    //     }
    //     else
    //     {
    //       if( last_topo_point_id_ == topo_point_id )
    //       {
    //         planner_manager_->updateTopoPoint(map_topo_points_[last_topo_point_id_]);
    //         have_new_target_ = false;
    //       }
    //     }
    //   }
    // }

    int topo_point_id;
    topo_point_id = updateNewTopoPoint( start_pt_, local_target_pt_ );
    if( topo_point_id >= 0 )
      planner_manager_->updateTopoPoint(map_topo_points_[topo_point_id]);

    bool plan_success = planner_manager_->reboundReplan(
        start_pt_, start_vel_, start_acc_,
        local_target_pt_, local_target_vel_,
        (have_new_target_ || flag_use_poly_init),
        flag_randomPolyTraj, touch_goal_);

    have_new_target_ = false;

    if (plan_success)
    {

      traj_utils::PolyTraj poly_msg;
      traj_utils::MINCOTraj MINCO_msg;
      polyTraj2ROSMsg(poly_msg, MINCO_msg);
      poly_traj_pub_.publish(poly_msg);
      broadcast_ploytraj_pub_.publish(MINCO_msg);
    }

    return plan_success;
  }

  bool EGOReplanFSM::planFromGlobalTraj(const int trial_times /*=1*/) //zx-todo
  {
    start_pt_ = odom_pos_;
    start_vel_ = odom_vel_;
    start_acc_.setZero();


    bool flag_random_poly_init;
    if (timesOfConsecutiveStateCalls().first == 1)
      flag_random_poly_init = false;
    else
      flag_random_poly_init = true;

    for (int i = 0; i < trial_times; i++)
    {
      if (callReboundReplan(true, flag_random_poly_init))
      {
        return true;
      }
    }
    return false;
  }

  bool EGOReplanFSM::planFromLocalTraj(const int trial_times /*=1*/)
  {

    LocalTrajData *info = &planner_manager_->traj_.local_traj;
    double t_cur = ros::Time::now().toSec() - info->start_time;

    if(static_debug_)
    {
      start_pt_ = odom_pos_;
      start_vel_ = odom_vel_;
      start_acc_.setZero();
    }
    else
    {
      start_pt_ = info->traj.getPos(t_cur);
      start_vel_ = info->traj.getVel(t_cur);
      start_acc_ = info->traj.getAcc(t_cur);
    }
    
    // start_pt_.setZero();
    // start_vel_.setZero();
    // start_acc_.setZero();

    // std::cout << (start_pt_ - odom_pos_).norm() << std::endl;
    // std::cout << (start_vel_ - odom_vel_).norm() << std::endl;

    // 进的是这个
    bool success = callReboundReplan(false, false);

    if (!success)
    {
      success = callReboundReplan(true, false);
      if (!success)
      {
        for (int i = 0; i < trial_times; i++)
        {
          success = callReboundReplan(true, true);
          if (success)
            break;
        }
        if (!success)
        {
          return false;
        }
      }
    }

    return true;
  }

  bool EGOReplanFSM::planNextWaypoint(const Eigen::Vector3d next_wp)
  {
    bool success = false;
    std::vector<Eigen::Vector3d> one_pt_wps;
    one_pt_wps.push_back(next_wp);
    success = planner_manager_->planGlobalTrajWaypoints(
        odom_pos_, odom_vel_, Eigen::Vector3d::Zero(),
        one_pt_wps, Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero());

    // visualization_->displayGoalPoint(next_wp, Eigen::Vector4d(0, 0.5, 0.5, 1), 0.3, 0);

    if (success)
    {
      end_pt_ = next_wp;

      /*** display ***/
      constexpr double step_size_t = 0.1;
      int i_end = floor(planner_manager_->traj_.global_traj.duration / step_size_t);
      vector<Eigen::Vector3d> gloabl_traj(i_end);
      for (int i = 0; i < i_end; i++)
      {
        gloabl_traj[i] = planner_manager_->traj_.global_traj.traj.getPos(i * step_size_t);
      }

      have_target_ = true;
      have_new_target_ = true;

      /*** FSM ***/
      if (exec_state_ != WAIT_TARGET && exec_state_ != RC_IN_DEAD_ZONE)
      {
        while (exec_state_ != EXEC_TRAJ)
        {
          ros::spinOnce();
          ros::Duration(0.001).sleep();
        }
        changeFSMExecState(REPLAN_TRAJ, "TRIG");
      }

      // visualization_->displayGoalPoint(end_pt_, Eigen::Vector4d(1, 0, 0, 1), 0.3, 0);
      visualization_->displayGlobalPathList(gloabl_traj, 0.1, 0);
    }
    else
    {
      ROS_ERROR("Unable to generate global trajectory!");
    }

    return success;
  }

  bool EGOReplanFSM::checkCollision( Eigen::Vector3d start_p, Eigen::Vector3d vec, double check_length, double &collision_length )
  {
    // 给起点和方向，给长度，查看在哪里撞了，和撞得地方的长度
    Eigen::Vector3d norm_vec = vec;
    Eigen::Vector3d temp_point = start_p;

    double res = planner_manager_->grid_map_->getResolution() * 0.5;
    norm_vec = norm_vec * res;
    for( double i = res; i < check_length; i = i + res )
    {
      temp_point = temp_point + norm_vec;
      collision_length = i;
      if(planner_manager_->grid_map_->getInflateOccupancy(temp_point) != 0)
      {
        return true;
      }
      // std::cout << temp_point.transpose() << std::endl;
    }
    // std::cout << collision_length << std::endl;
    return false;
  }
  
  void EGOReplanFSM::checkGoal(Eigen::Vector3d odom_p, Eigen::Vector3d &end_p)
  {
    double max_length = 0;
    Eigen::Vector3d max_length_vec;
    Eigen::Vector3d collision_point;
    Eigen::Vector3d check_vec = end_p - odom_p;
    double collision_length, check_length;
    check_length = check_vec.norm();
    check_vec.normalize();
    if( checkCollision( odom_p, check_vec, check_length, collision_length ) )
    {
      Eigen::Vector3d collision_point = odom_p + collision_length * check_vec;
      // 对上下左右都进行一遍check，然后选出一个最合适的方向作为终点
      double size = 0.1 * collision_length;
      if( size > 0.15 )
        size = 0.15;
      int size_i = 2;

      max_length = collision_length;
      max_length_vec = check_vec;

      for( int z = -size_i; z <= size_i; z++ )
      {
        for( int y = -size_i; y <= size_i; y++ )
        {
          for( int x = -size_i; x <= size_i; x++ )
          {
            Eigen::Vector3d xyz_point = Eigen::Vector3d{ x * size, y * size, z * size };
            Eigen::Vector3d temp_point = collision_point + xyz_point;
            check_vec = temp_point - odom_p;
            check_vec.normalize();
            if( checkCollision( odom_p, check_vec, check_length, collision_length ) )
            {
              if( collision_length > max_length )
              {
                max_length = collision_length;
                max_length_vec = check_vec;
              }
            }
            else
            {
              // std::cout << "直接result: " << check_length << std::endl;
              end_p = odom_p + check_vec * check_length;
              // std::cout << "直接result: " << end_p.transpose() << std::endl;
              return;
            }
          }
        }
      }
      end_p = odom_p + max_length_vec * ( max_length - 1.0 );
    }
  }


  void EGOReplanFSM::waypointCallback(const quadrotor_msgs::GoalSetPtr &msg)
  {
    if (msg->drone_id != planner_manager_->pp_.drone_id || msg->goal[2] < -0.1)
      return;

    // ! check if the eye tracker line is in the occupency检查是否在障碍物里，然后
    // 只处理最近的障碍物的碰撞检查-检查是否在障碍物里，然后在周围生成一个方格空间进行check，选一个最长的即可
    // 如果发生了碰撞的话，要更新一下速度
    Eigen::Vector3d end_wp(msg->goal[0], msg->goal[1], msg->goal[2]);
    // end_wp[2] = 1.0;

    auto t1 = ros::Time::now();
    Eigen::Vector3d start_wp = odom_pos_;
    checkGoal(start_wp, end_wp);
    auto t2 = ros::Time::now();

    planner_manager_->updateGoal(start_wp, end_wp);

    double goal_length = (end_wp - start_wp).norm();

    // TODO 可以给的更讲道理一些
    if( goal_length < 1.0 )
    {
      double desire_vel;
      desire_vel = goal_length;
      // planner_manager_->updatePlanParam( desire_vel );
    }

    if( static_debug_ )
    {
      if (planNextWaypoint(end_wp))
      {
        have_trigger_ = true;
      }
      return;
    }

    // 如果太短了就不发新goal
    if( goal_length < 1.0 )
      return;

    // 如果新老goal差了不到1m就不发了
    if( (end_wp - planner_manager_->last_local_target_pt_).norm() > 1.0 )
    {
      ROS_INFO("Check Goal Time(s): %f",(t2-t1).toSec());
      ROS_INFO("Received goal: %f, %f, %f",msg->goal[0], msg->goal[1], msg->goal[2]);
      ROS_INFO("Send goal: %f, %f, %f", end_wp[0], end_wp[1], end_wp[2] );
      if (planNextWaypoint(end_wp))
      {
        have_trigger_ = true;
      }
    }
    else
    {
      // 如果飞机距离障碍物太近了强制replan
      double dist;
      planner_manager_->grid_map_->evaluateESDF( start_wp, dist );
      if( dist < 1.0 )
      {
        ROS_INFO("Send goal: %f, %f, %f", end_wp[0], end_wp[1], end_wp[2] );
        if (planNextWaypoint(end_wp))
        {
          have_trigger_ = true;
        }
      }
    }
  }

  // void EGOReplanFSM::planGlobalTrajbyGivenWps()
  // {
  //   std::vector<Eigen::Vector3d> wps(waypoint_num_);
  //   for (int i = 0; i < waypoint_num_; i++)
  //   {
  //     wps[i](0) = waypoints_[i][0];
  //     wps[i](1) = waypoints_[i][1];
  //     wps[i](2) = waypoints_[i][2];

  //     end_pt_ = wps.back();
  //   }
  //   bool success = planner_manager_->planGlobalTrajWaypoints(odom_pos_, Eigen::Vector3d::Zero(),
  //                                                            Eigen::Vector3d::Zero(), wps,
  //                                                            Eigen::Vector3d::Zero(),
  //                                                            Eigen::Vector3d::Zero());

  //   for (size_t i = 0; i < (size_t)waypoint_num_; i++)
  //   {
  //     visualization_->displayGoalPoint(wps[i], Eigen::Vector4d(0, 0.5, 0.5, 1), 0.3, i);
  //     ros::Duration(0.001).sleep();
  //   }

  //   if (success)
  //   {

  //     /*** display ***/
  //     constexpr double step_size_t = 0.1;
  //     int i_end = floor(planner_manager_->traj_.global_traj.duration / step_size_t);
  //     std::vector<Eigen::Vector3d> gloabl_traj(i_end);
  //     for (int i = 0; i < i_end; i++)
  //     {
  //       gloabl_traj[i] = planner_manager_->traj_.global_traj.traj.getPos(i * step_size_t);
  //     }

  //     have_target_ = true;
  //     have_new_target_ = true;

  //     // visualization_->displayGoalPoint(end_pt_, Eigen::Vector4d(1, 0, 0, 1), 0.3, 0);
  //     ros::Duration(0.001).sleep();
  //     visualization_->displayGlobalPathList(gloabl_traj, 0.1, 0);
  //     ros::Duration(0.001).sleep();
  //   }
  //   else
  //   {
  //     ROS_ERROR("Unable to generate global trajectory!");
  //   }
  // }

  void EGOReplanFSM::readGivenWpsAndPlan()
  {
    if (waypoint_num_ <= 0)
    {
      ROS_ERROR("Wrong waypoint_num_ = %d", waypoint_num_);
      return;
    }

    wps_.resize(waypoint_num_);
    for (int i = 0; i < waypoint_num_; i++)
    {
      wps_[i](0) = waypoints_[i][0];
      wps_[i](1) = waypoints_[i][1];
      wps_[i](2) = waypoints_[i][2];
    }

    // bool success = planner_manager_->planGlobalTrajWaypoints(
    //   odom_pos_, Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(),
    //   wps_, Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero());

    for (size_t i = 0; i < (size_t)waypoint_num_; i++)
    {
      visualization_->displayGoalPoint(wps_[i], Eigen::Vector4d(0, 0.5, 0.5, 1), 0.3, i);
      ros::Duration(0.001).sleep();
    }

    // plan first global waypoint
    wpt_id_ = 0;
    planNextWaypoint(wps_[wpt_id_]);
  }

  void EGOReplanFSM::mandatoryStopCallback(const std_msgs::Empty &msg)
  {
    mandatory_stop_ = true;
    ROS_ERROR("Received a mandatory stop command!");
    changeFSMExecState(EMERGENCY_STOP, "Mandatory Stop");
    enable_fail_safe_ = false;
  }

  void EGOReplanFSM::odometryCallback(const nav_msgs::OdometryConstPtr &msg)
  {
    odom_pos_(0) = msg->pose.pose.position.x;
    odom_pos_(1) = msg->pose.pose.position.y;
    odom_pos_(2) = msg->pose.pose.position.z;

    Eigen::Quaterniond odom_orient;
    odom_orient.w() = msg->pose.pose.orientation.w;
    odom_orient.x() = msg->pose.pose.orientation.x;
    odom_orient.y() = msg->pose.pose.orientation.y;
    odom_orient.z() = msg->pose.pose.orientation.z;
    Eigen::Vector3d rot_x = odom_orient.toRotationMatrix().block(0, 0, 3, 1);
    odom_yaw_ = atan2(rot_x(1), rot_x(0));

    odom_vel_(0) = msg->twist.twist.linear.x;
    odom_vel_(1) = msg->twist.twist.linear.y;
    odom_vel_(2) = msg->twist.twist.linear.z;

    have_odom_ = true;
  }

  void EGOReplanFSM::triggerCallback(const geometry_msgs::PoseStampedPtr &msg)
  {
    have_trigger_ = true;
    cout << "Triggered!" << endl;
  }

  void EGOReplanFSM::RecvBroadcastMINCOTrajCallback(const traj_utils::MINCOTrajConstPtr &msg)
  {
    if (msg->drone_id < 0)
    {
      ROS_ERROR("drone_id < 0 is not allowed in a swarm system!");
      return;
    }
    if (msg->order != 5)
    {
      ROS_ERROR("Only support trajectory order equals 5 now!");
      return;
    }
    if (msg->duration.size() != (msg->inner_x.size() + 1))
    {
      ROS_ERROR("WRONG trajectory parameters.");
      return;
    }
    if (abs((ros::Time::now() - msg->start_time).toSec()) > 0.25)
    {
      ROS_WARN("Time stamp diff: Local - Remote Agent %d = %fs",
               msg->drone_id, (ros::Time::now() - msg->start_time).toSec());
      return;
    }

    const size_t recv_id = (size_t)msg->drone_id;
    if ((int)recv_id == planner_manager_->pp_.drone_id)
      return;

    /* Fill up the buffer */
    if (planner_manager_->traj_.swarm_traj.size() <= recv_id)
    {
      for (size_t i = planner_manager_->traj_.swarm_traj.size(); i <= recv_id; i++)
      {
        LocalTrajData blank;
        blank.drone_id = -1;
        planner_manager_->traj_.swarm_traj.push_back(blank);
      }
    }

    /* Store data */
    planner_manager_->traj_.swarm_traj[recv_id].drone_id = recv_id;
    planner_manager_->traj_.swarm_traj[recv_id].traj_id = msg->traj_id;
    planner_manager_->traj_.swarm_traj[recv_id].start_time = msg->start_time.toSec();

    int piece_nums = msg->duration.size();
    Eigen::Matrix<double, 3, 3> headState, tailState;
    headState << msg->start_p[0], msg->start_v[0], msg->start_a[0],
        msg->start_p[1], msg->start_v[1], msg->start_a[1],
        msg->start_p[2], msg->start_v[2], msg->start_a[2];
    tailState << msg->end_p[0], msg->end_v[0], msg->end_a[0],
        msg->end_p[1], msg->end_v[1], msg->end_a[1],
        msg->end_p[2], msg->end_v[2], msg->end_a[2];
    Eigen::MatrixXd innerPts(3, piece_nums - 1);
    Eigen::VectorXd durations(piece_nums);
    for (int i = 0; i < piece_nums - 1; i++)
      innerPts.col(i) << msg->inner_x[i], msg->inner_y[i], msg->inner_z[i];
    for (int i = 0; i < piece_nums; i++)
      durations(i) = msg->duration[i];
    poly_traj::MinJerkOpt MJO;
    MJO.reset(headState, tailState, piece_nums);
    MJO.generate(innerPts, durations);

    poly_traj::Trajectory trajectory = MJO.getTraj();
    planner_manager_->traj_.swarm_traj[recv_id].traj = trajectory;

    planner_manager_->traj_.swarm_traj[recv_id].duration = trajectory.getTotalDuration();
    planner_manager_->traj_.swarm_traj[recv_id].start_pos = trajectory.getPos(0.0);

    /* Check Collision */
    if (planner_manager_->checkCollision(recv_id))
    {
      changeFSMExecState(REPLAN_TRAJ, "SWARM_CHECK");
    }

    /* Check if receive agents have lower drone id */
    if (!have_recv_pre_agent_)
    {
      if ((int)planner_manager_->traj_.swarm_traj.size() >= planner_manager_->pp_.drone_id)
      {
        for (int i = 0; i < planner_manager_->pp_.drone_id; ++i)
        {
          if (planner_manager_->traj_.swarm_traj[i].drone_id != i)
          {
            break;
          }

          have_recv_pre_agent_ = true;
        }
      }
    }
  }

  void EGOReplanFSM::polyTraj2ROSMsg(traj_utils::PolyTraj &poly_msg, traj_utils::MINCOTraj &MINCO_msg)
  {

    auto data = &planner_manager_->traj_.local_traj;
    Eigen::VectorXd durs = data->traj.getDurations();
    int piece_num = data->traj.getPieceNum();

    poly_msg.drone_id = planner_manager_->pp_.drone_id;
    poly_msg.traj_id = data->traj_id;
    poly_msg.start_time = ros::Time(data->start_time);
    poly_msg.order = 5; // todo, only support order = 5 now.
    poly_msg.duration.resize(piece_num);
    poly_msg.coef_x.resize(6 * piece_num);
    poly_msg.coef_y.resize(6 * piece_num);
    poly_msg.coef_z.resize(6 * piece_num);
    for (int i = 0; i < piece_num; ++i)
    {
      poly_msg.duration[i] = durs(i);

      poly_traj::CoefficientMat cMat = data->traj.getPiece(i).getCoeffMat();
      int i6 = i * 6;
      for (int j = 0; j < 6; j++)
      {
        poly_msg.coef_x[i6 + j] = cMat(0, j);
        poly_msg.coef_y[i6 + j] = cMat(1, j);
        poly_msg.coef_z[i6 + j] = cMat(2, j);
      }
    }

    MINCO_msg.drone_id = planner_manager_->pp_.drone_id;
    MINCO_msg.traj_id = data->traj_id;
    MINCO_msg.start_time = ros::Time(data->start_time);
    MINCO_msg.order = 5; // todo, only support order = 5 now.
    MINCO_msg.duration.resize(piece_num);
    Eigen::Vector3d vec;
    vec = data->traj.getPos(0);
    MINCO_msg.start_p[0] = vec(0), MINCO_msg.start_p[1] = vec(1), MINCO_msg.start_p[2] = vec(2);
    vec = data->traj.getVel(0);
    MINCO_msg.start_v[0] = vec(0), MINCO_msg.start_v[1] = vec(1), MINCO_msg.start_v[2] = vec(2);
    vec = data->traj.getAcc(0);
    MINCO_msg.start_a[0] = vec(0), MINCO_msg.start_a[1] = vec(1), MINCO_msg.start_a[2] = vec(2);
    vec = data->traj.getPos(data->duration);
    MINCO_msg.end_p[0] = vec(0), MINCO_msg.end_p[1] = vec(1), MINCO_msg.end_p[2] = vec(2);
    vec = data->traj.getVel(data->duration);
    MINCO_msg.end_v[0] = vec(0), MINCO_msg.end_v[1] = vec(1), MINCO_msg.end_v[2] = vec(2);
    vec = data->traj.getAcc(data->duration);
    MINCO_msg.end_a[0] = vec(0), MINCO_msg.end_a[1] = vec(1), MINCO_msg.end_a[2] = vec(2);
    MINCO_msg.inner_x.resize(piece_num - 1);
    MINCO_msg.inner_y.resize(piece_num - 1);
    MINCO_msg.inner_z.resize(piece_num - 1);
    Eigen::MatrixXd pos = data->traj.getPositions();
    for (int i = 0; i < piece_num - 1; i++)
    {
      MINCO_msg.inner_x[i] = pos(0, i + 1);
      MINCO_msg.inner_y[i] = pos(1, i + 1);
      MINCO_msg.inner_z[i] = pos(2, i + 1);
    }
    for (int i = 0; i < piece_num; i++)
      MINCO_msg.duration[i] = durs[i];
  }
} // namespace ego_planner
