#! /usr/bin/env python
import math
import numpy as np
import rospy

from sensor_msgs.msg import PointCloud2, PointField
from sensor_msgs import point_cloud2
from nav_msgs.msg import Odometry


class map_generator(object):

    def __init__(self):
        """ Constructor """
        rospy.init_node('random_map_sensing', anonymous=True)
        self.map = None
        self.topo_points_msg = None
        self.points = []
        self.points_temp = []
        self.topo_points = []
        self.topo_points_temp = []
        self.has_odom = False
        self.has_map = False
        self.init_params()
        self.floor_bias = -0.5

    def init_params(self):
        """ Initializes ros parameters """
        
        # rospack = rospkg.RosPack()
        # package_path = rospack.get_path("yolov4_trt_ros")
        self.dimx = rospy.get_param("map/x_size", default=10.0)
        self.dimy = rospy.get_param("map/y_size", default=10.0)
        self.dimz = rospy.get_param("map/z_size", default=3.0)

        self.resolution = rospy.get_param("map/resolution", default=0.05)

        self.add_floor = rospy.get_param("map/add_floor", default=True)
        self.add_ceiling = rospy.get_param("map/add_ceiling", default=True)
        
        self.all_map_topic = rospy.get_param("all_map_topic", default="/map_generator/global_cloud")
        self.topo_points_topic = rospy.get_param("topo_points_topic", default="/map_generator/topo_points")
        self.all_map_pub = rospy.Publisher(self.all_map_topic, PointCloud2, queue_size=1)
        self.topo_points_pub = rospy.Publisher(self.topo_points_topic, PointCloud2, queue_size=1)
        # self.odom_sub = rospy.Subscriber( "odometry", Odometry, self.odom_callback, queue_size=50);

        self.rate = rospy.get_param("sensing/rate", default=1.0)
        self.rate = rospy.Rate(self.rate)

        print("dimx: ", self.dimx)
        print("dimy: ", self.dimy)
        print("dimz: ", self.dimz)

    def add_box(self, size, position):
        ''' size: [x,y,z]
            position: [x,y,z] --- center position
        '''
        position[2] -= self.floor_bias
        x_low = math.floor((position[0] - size[0] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        x_high = math.floor((position[0] + size[0] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        y_low = math.floor((position[1] - size[1] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        y_high = math.floor((position[1] + size[1] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        z_low = math.floor((position[2] - size[2] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        z_high = math.floor((position[2] + size[2] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution

        x = x_low
        while x <= x_high:
            y = y_low
            while y <= y_high:
                z = z_low
                while z <= z_high:
                    if (math.fabs(x - x_low) < self.resolution) or (math.fabs(x - x_high) < self.resolution) \
                        or (math.fabs(y - y_low) < self.resolution) or (math.fabs(y - y_high) < self.resolution) \
                        or (math.fabs(z - z_low) < self.resolution) or (math.fabs(z - z_high) < self.resolution):
                        
                        self.points_temp.append([x,y,z])
                    z += self.resolution
                y += self.resolution
            x += self.resolution

        return

    def add_cylinder(self, size, position):
        ''' size: [r, h]
            position: [x,y,z] --- center position
        '''
        position[2] -= self.floor_bias
        center_x = position[0]
        center_y = position[1]
        z_low = math.floor((position[2] - size[1] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution
        z_high = math.floor((position[2] + size[1] / 2) / self.resolution) * self.resolution  + 0.5 * self.resolution

        radius_num = math.floor(size[0] / self.resolution)
        x = - radius_num
        while x <= radius_num:
            y = - radius_num
            while y <= radius_num:
                radius2 = x ** 2 + y ** 2
                if radius2 < (radius_num + 0.5) ** 2:
                    z = z_low
                    while z <= z_high:
                        if radius2 > (radius_num - 0.5) ** 2 or \
                            (math.fabs(z - z_low) < self.resolution) or (math.fabs(z - z_high) < self.resolution):
                            self.points.append([center_x + x * self.resolution, center_y + y * self.resolution, z])
                        z += self.resolution
                y += 1
            x += 1

        return

    def add_racing_ring(self, ring_size, position):
        '''ring_size: [x, y] '''
        '''position: [x, y, z] '''
        # up
        self.add_box([0.25, 0.5 + ring_size[0], 0.25], [position[0], position[1], position[2] + 0.125 + ring_size[1] / 2.0])
        # down
        self.add_box([0.25, 0.5 + ring_size[0], 0.25], [position[0], position[1], position[2] - 0.125 - ring_size[1] / 2.0])
        # left
        self.add_box([0.25, 0.25, 0.5 + ring_size[0]], [position[0], position[1] - 0.125 - ring_size[0] / 2.0, position[2]])
        # right
        self.add_box([0.25, 0.25, 0.5 + ring_size[0]], [position[0], position[1] + 0.125 + ring_size[0] / 2.0, position[2]])

        position[2] = position[2] + ring_size[1] / 2.0
        self.topo_points_temp.append(position)


    def spin_current_map(self, spin_axis, spin_angle):
        '''spin_axis: [x, y] '''
        '''spin_angle: angle '''
        if spin_angle != 0.0:
            for item in self.points_temp:
                x_temp = item[0] - spin_axis[0]
                y_temp = item[1] - spin_axis[1]
                item[0] = x_temp * np.cos(spin_angle) - y_temp * np.sin(spin_angle) + spin_axis[0]
                item[1] = x_temp * np.sin(spin_angle) + y_temp * np.cos(spin_angle) + spin_axis[1]
            for item in self.topo_points_temp:
                x_temp = item[0] - spin_axis[0]
                y_temp = item[1] - spin_axis[1]
                item[0] = x_temp * np.cos(spin_angle) - y_temp * np.sin(spin_angle) + spin_axis[0]
                item[1] = x_temp * np.sin(spin_angle) + y_temp * np.cos(spin_angle) + spin_axis[1]

        self.points += self.points_temp
        self.points_temp = []
        self.topo_points += self.topo_points_temp
        self.topo_points_temp = []


    def publish_map(self):
        if not self.has_map:
            return False
        self.all_map_pub.publish(self.map)
        self.topo_points_pub.publish(self.topo_points_msg)
        return True

    def odom_callback(self, odom):
        self.has_odom = True
        self.publish_map()


    def make_map(self):
        rospy.loginfo("start making map")

        self.add_racing_ring([1.0,1.0], [1.0, 0.0, 0.5])
        self.add_racing_ring([1.0,1.0], [1.0, 1.5, 0.5])
        self.add_racing_ring([1.0,1.0], [1.0, -1.5, 0.5])
        self.spin_current_map([0.0, 0.0], 0.0)

        self.add_racing_ring([1.0,1.0], [3.5, 0.0, 0.5])
        self.add_racing_ring([1.0,1.0], [3.5, 1.5, 0.5])
        self.add_racing_ring([1.0,1.0], [3.5, -1.5, 0.5])
        self.spin_current_map([3.5, 0.0], np.pi / 6.0)

        self.add_racing_ring([1.0,1.0], [6.0, 0.0, 0.5])
        self.add_racing_ring([1.0,1.0], [6.0, 1.5, 0.5])
        self.add_racing_ring([1.0,1.0], [6.0, -1.5, 0.5])
        self.spin_current_map([0.0, 0.0], 0.0)

        self.add_racing_ring([1.0,1.0], [8.0, 0.0, 0.5])
        self.add_racing_ring([1.0,1.0], [8.0, 1.5, 0.5])
        self.add_racing_ring([1.0,1.0], [8.0, -1.5, 0.5])
        self.spin_current_map([8.0, -2.0], -np.pi / 6.0)

        self.add_racing_ring([1.0,1.0], [10.5, 0.0, 0.5])
        self.add_racing_ring([1.0,1.0], [10.5, 1.5, 0.5])
        self.add_racing_ring([1.0,1.0], [10.5, -1.5, 0.5])
        self.spin_current_map([8.0, -2.0], -np.pi / 3.0)

        # transfer to pcl
        self.map = PointCloud2()
        self.map.height = 1
        self.map.width = len(self.points)
        self.map.fields = [
            PointField('x', 0, PointField.FLOAT32, 1),
            PointField('y', 4, PointField.FLOAT32, 1),
            PointField('z', 8, PointField.FLOAT32, 1)]
        self.map.point_step = 12 #12
        self.map.row_step = 12 * len(self.points)
        self.map.is_bigendian = False
        self.map.is_dense = False
        self.map.data = np.asarray(self.points, np.float32).tostring()
        self.map.header.frame_id = "world"

        self.topo_points_msg = PointCloud2()
        self.topo_points_msg.height = 1
        self.topo_points_msg.width = len(self.topo_points)
        self.topo_points_msg.fields = [
            PointField('x', 0, PointField.FLOAT32, 1),
            PointField('y', 4, PointField.FLOAT32, 1),
            PointField('z', 8, PointField.FLOAT32, 1)]
        self.topo_points_msg.point_step = 12 #12
        self.topo_points_msg.row_step = 12 * len(self.topo_points)
        self.topo_points_msg.is_bigendian = False
        self.topo_points_msg.is_dense = False
        self.topo_points_msg.data = np.asarray(self.topo_points, np.float32).tostring()
        self.topo_points_msg.header.frame_id = "world"

        self.has_map = True
        rospy.loginfo("finish making map")

        return True
    
def main():
    

    map_maker = map_generator()
    map_maker.make_map()
    while not rospy.is_shutdown():
        map_maker.publish_map()
        map_maker.rate.sleep()

if __name__ == '__main__':
    main()

