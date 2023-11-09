import pynput
import rospy
import cv2
from geometry_msgs.msg import PointStamped
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import numpy as np
import threading
import os
import signal



scroll_trigger = 0
speed = 0.5
bridge = CvBridge()
x_last = -1
y_last = -1
action_last = -1
is_lect_button_pressed = 0
bottom_img = cv2.imread(
    "src/gaze_sim/bottom.png", cv2.IMREAD_COLOR)
is_init = 0

# !!!Note: Here, point.x = X coordinate of gaze on image plane
#                point.y = Y coordinate of gaze on image plane
#                point.z = desired speed
gaze_pub = rospy.Publisher('/eye_tracker/gaze', PointStamped, queue_size=10)
gaze_msg = PointStamped()
gaze_msg.header.frame_id = "world"


def on_scroll(x, y, dx, dy):
    global speed
    if dy > 0:
        speed = speed + 0.1
    elif dy < 0:
        speed = speed - 0.1

    if speed > 2:
        speed = 2
    elif speed < 0:
        speed = 0


def ScrollListener(args):
    with pynput.mouse.Listener(
            on_scroll=on_scroll) as listener:
        listener.join()


t_scroll = threading.Thread(target=ScrollListener, args=(1,))


def handler(signum, frame):
    if t_scroll.is_alive():
        os._exit(1)


def MouseCallback(action, x, y, flags, *userdata):
    if 0 <= x < 640 and 480 > y >= 0:
        gaze_msg.point.x = x
        gaze_msg.point.y = y

    global is_lect_button_pressed
    if action == cv2.EVENT_LBUTTONDOWN:
        is_lect_button_pressed = 1
        # print("Start executing trajectory")
    if action == cv2.EVENT_LBUTTONUP:
        is_lect_button_pressed = 0
        # print("Stop!")


def fpv_img_callback(msg):
    global is_lect_button_pressed
    global bottom_img
    global is_init

    fpv_img = bridge.imgmsg_to_cv2(msg, "passthrough")
    cv2.normalize(fpv_img, fpv_img, 0, 255, cv2.NORM_MINMAX)
    fpv_img = np.uint8(fpv_img)
    fpv_img = 255 - fpv_img
    ret, fpv_img = cv2.threshold(fpv_img, 254, 255, cv2.THRESH_TOZERO_INV)
    fpv_img = cv2.applyColorMap(fpv_img, cv2.COLORMAP_HOT)
    cv2.namedWindow('Display1', cv2.WINDOW_AUTOSIZE)
    resize_bottom = (fpv_img.shape[1], int(
        fpv_img.shape[1] / bottom_img.shape[1] * bottom_img.shape[0]))
    bottom_img = cv2.resize(bottom_img, resize_bottom,
                            interpolation=cv2.INTER_AREA)
    # print()
    fpv_img = np.vstack((fpv_img, bottom_img))
    # Put text
    font = cv2.FONT_HERSHEY_SIMPLEX
    org = (20, 520)
    fontScale = 0.7
    color = (0, 0, 0)
    # Line thickness of 2 px
    thickness = 2

    if is_lect_button_pressed == 1:
        gaze_msg.point.z = speed
        fpv_img = cv2.putText(fpv_img, 'Start! Scroll to adjust speed.', org, font,
                              fontScale, color, thickness, cv2.LINE_AA)
    else:
        gaze_msg.point.z = 0
        fpv_img = cv2.putText(fpv_img, 'Stop! Hold down left button to start.',
                              org, font, fontScale, color, thickness, cv2.LINE_AA)
    org = (40, 580)
    fpv_img = cv2.putText(fpv_img, 'Desired Speed: '+str(round(speed, 1))+' m/s',
                          org, font, fontScale, color, thickness, cv2.LINE_AA)
    cv2.imshow('Display1', fpv_img)
    cv2.waitKey(2)

    if is_init == 0:
        is_init = 1
        cv2.setMouseCallback("Display1", MouseCallback, 0)
        t_scroll.start()

    gaze_pub.publish(gaze_msg)


def main():
    rospy.init_node('gaze_sim', anonymous=True)
    rospy.Subscriber("/drone_0_pcl_render_node/colordepth", Image, fpv_img_callback)
    rospy.Publisher('/eye_tracker/gaze', PointStamped, queue_size=10)
    rospy.spin()


if __name__ == '__main__':
    main()
    signal.signal(signal.SIGINT, handler)
