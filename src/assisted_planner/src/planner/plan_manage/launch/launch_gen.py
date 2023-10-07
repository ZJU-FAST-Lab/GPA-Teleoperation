import sys
import numpy as np
import os
import math

fname = "/home/zx/workspace/DSTO-Swarm/src/planner/plan_manage/launch/test.launch"
def main(argv):
	num = int(argv[1])
	str_begin = "<launch>\n\
	    <arg name=\"map_size_x\" value=\"40.0\"/>\n\
	    <arg name=\"map_size_y\" value=\"40.0\"/>\n\
    	<arg name=\"map_size_z\" value=\" 4.0\"/>\n\
    	<arg name=\"odom_topic\" value=\"visual_slam/odom\" />\n\
    	<!-- swarm topic transmitter bridge-->\n\
    	<include file=\"$(find swarm_bridge)/launch/bridge.launch\">\n\
        <arg name=\"drone_id\" value=\"999\"/>\n\
        <arg name=\"broadcast_ip\" value=\"127.0.0.255\"/>\n\
    	</include>\n\
    	<!-- map --> \n\
		<node pkg =\"map_generator\" name =\"random_forest\" type =\"random_forest\" output = \"screen\">\n\
        <param name=\"map/x_size\"     value=\"26\" />\n\
        <param name=\"map/y_size\"     value=\"20\" />\n\
        <param name=\"map/z_size\"     value=\"3\" />\n\
        <param name=\"map/resolution\" value=\"0.1\"/>\n\
        <param name=\"ObstacleShape/seed\" value=\"1\"/>\n\
        <param name=\"map/obs_num\"    value=\"100\"/>\n\
        <param name=\"ObstacleShape/lower_rad\" value=\"0.5\"/>\n\
        <param name=\"ObstacleShape/upper_rad\" value=\"0.7\"/>\n\
        <param name=\"ObstacleShape/lower_hei\" value=\"0.0\"/>\n\
        <param name=\"ObstacleShape/upper_hei\" value=\"3.0\"/>\n\
        <param name=\"map/circle_num\" value=\"100\"/>\n\
        <param name=\"ObstacleShape/radius_l\" value=\"0.7\"/>\n\
        <param name=\"ObstacleShape/radius_h\" value=\"0.5\"/>\n\
        <param name=\"ObstacleShape/z_l\" value=\"0.7\"/>\n\
        <param name=\"ObstacleShape/z_h\" value=\"0.8\"/>\n\
        <param name=\"ObstacleShape/theta\" value=\"0.5\"/>\n\
        <param name=\"pub_rate\"   value=\"1.0\"/>\n\
        <param name=\"min_distance\" value=\"0.8\"/>\n\
    </node>"
	str_end = "</launch>\n"

	
	height = 1.5
	theta = np.linspace(-3.14159, 3.14159, num+1)
	r = 12
	file = open(fname, "w")
	file.write(str_begin)
	for i in range(num):
		str_for_agent = "    <include file=\"$(find ego_planner)/launch/run_in_sim.launch\">\n\
			<arg name=\"drone_id\"   value=\"{drone_id}\"/>\n\
			<arg name=\"init_x\"     value=\"{init_x}\"/>\n\
			<arg name=\"init_y\"     value=\"{init_y}\"/>\n\
			<arg name=\"init_z\"     value=\"{init_z}\"/>\n\
			<arg name=\"target_x\"   value=\"{target_x}\"/>\n\
			<arg name=\"target_y\"   value=\"{target_y}\"/>\n\
			<arg name=\"target_z\"   value=\"{target_z}\"/>\n\
			<arg name=\"map_size_x\" value=\"$(arg map_size_x)\"/>\n\
			<arg name=\"map_size_y\" value=\"$(arg map_size_y)\"/>\n\
			<arg name=\"map_size_z\" value=\"$(arg map_size_z)\"/>\n\
			<arg name=\"odom_topic\" value=\"$(arg odom_topic)\"/>\n\
		</include>\n".format(drone_id=i, init_x=r * math.sin(theta[i]), init_y=r * math.cos(theta[i]), init_z=height, \
		target_x=-r * math.sin(theta[i]), target_y=-r * math.cos(theta[i]), target_z=height) 
		print("({x},{y},{z})".format(x=r * math.sin(theta[i]), y=r * math.cos(theta[i]), z=height))
		file.write(str_for_agent)
	file.write(str_end)
	file.close()

	# height = 1.5
	# x_s = -5
	# x_e = 5
	# y = np.linspace(-5, 5, num)
	# file = open(fname, "w")
	# file.write(str_begin)
	# for i in range(num):
	# 	str_for_agent = "    <include file=\"$(find ego_planner)/launch/run_in_sim.launch\">\n\
	# 		<arg name=\"drone_id\"   value=\"{drone_id}\"/>\n\
	# 		<arg name=\"init_x\"     value=\"{init_x}\"/>\n\
	# 		<arg name=\"init_y\"     value=\"{init_y}\"/>\n\
	# 		<arg name=\"init_z\"     value=\"{init_z}\"/>\n\
	# 		<arg name=\"target_x\"   value=\"{target_x}\"/>\n\
	# 		<arg name=\"target_y\"   value=\"{target_y}\"/>\n\
	# 		<arg name=\"target_z\"   value=\"{target_z}\"/>\n\
	# 		<arg name=\"map_size_x\" value=\"$(arg map_size_x)\"/>\n\
	# 		<arg name=\"map_size_y\" value=\"$(arg map_size_y)\"/>\n\
	# 		<arg name=\"map_size_z\" value=\"$(arg map_size_z)\"/>\n\
	# 		<arg name=\"odom_topic\" value=\"$(arg odom_topic)\"/>\n\
	# 	</include>\n".format(drone_id=i, init_x=x_s, init_y=y[i], init_z=height, \
	# 	target_x=x_e, target_y=y[num-i-1], target_z=height) 
	# 	file.write(str_for_agent)
	# file.write(str_end)
	# file.close()

if __name__ == '__main__':
	main(sys.argv)
