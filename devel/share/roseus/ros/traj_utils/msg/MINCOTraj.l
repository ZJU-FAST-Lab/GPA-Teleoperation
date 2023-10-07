;; Auto-generated. Do not edit!


(when (boundp 'traj_utils::MINCOTraj)
  (if (not (find-package "TRAJ_UTILS"))
    (make-package "TRAJ_UTILS"))
  (shadow 'MINCOTraj (find-package "TRAJ_UTILS")))
(unless (find-package "TRAJ_UTILS::MINCOTRAJ")
  (make-package "TRAJ_UTILS::MINCOTRAJ"))

(in-package "ROS")
;;//! \htmlinclude MINCOTraj.msg.html


(defclass traj_utils::MINCOTraj
  :super ros::object
  :slots (_drone_id _traj_id _start_time _order _start_p _start_v _start_a _end_p _end_v _end_a _inner_x _inner_y _inner_z _duration ))

(defmethod traj_utils::MINCOTraj
  (:init
   (&key
    ((:drone_id __drone_id) 0)
    ((:traj_id __traj_id) 0)
    ((:start_time __start_time) (instance ros::time :init))
    ((:order __order) 0)
    ((:start_p __start_p) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:start_v __start_v) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:start_a __start_a) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:end_p __end_p) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:end_v __end_v) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:end_a __end_a) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:inner_x __inner_x) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:inner_y __inner_y) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:inner_z __inner_z) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:duration __duration) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _drone_id (round __drone_id))
   (setq _traj_id (round __traj_id))
   (setq _start_time __start_time)
   (setq _order (round __order))
   (setq _start_p __start_p)
   (setq _start_v __start_v)
   (setq _start_a __start_a)
   (setq _end_p __end_p)
   (setq _end_v __end_v)
   (setq _end_a __end_a)
   (setq _inner_x __inner_x)
   (setq _inner_y __inner_y)
   (setq _inner_z __inner_z)
   (setq _duration __duration)
   self)
  (:drone_id
   (&optional __drone_id)
   (if __drone_id (setq _drone_id __drone_id)) _drone_id)
  (:traj_id
   (&optional __traj_id)
   (if __traj_id (setq _traj_id __traj_id)) _traj_id)
  (:start_time
   (&optional __start_time)
   (if __start_time (setq _start_time __start_time)) _start_time)
  (:order
   (&optional __order)
   (if __order (setq _order __order)) _order)
  (:start_p
   (&optional __start_p)
   (if __start_p (setq _start_p __start_p)) _start_p)
  (:start_v
   (&optional __start_v)
   (if __start_v (setq _start_v __start_v)) _start_v)
  (:start_a
   (&optional __start_a)
   (if __start_a (setq _start_a __start_a)) _start_a)
  (:end_p
   (&optional __end_p)
   (if __end_p (setq _end_p __end_p)) _end_p)
  (:end_v
   (&optional __end_v)
   (if __end_v (setq _end_v __end_v)) _end_v)
  (:end_a
   (&optional __end_a)
   (if __end_a (setq _end_a __end_a)) _end_a)
  (:inner_x
   (&optional __inner_x)
   (if __inner_x (setq _inner_x __inner_x)) _inner_x)
  (:inner_y
   (&optional __inner_y)
   (if __inner_y (setq _inner_y __inner_y)) _inner_y)
  (:inner_z
   (&optional __inner_z)
   (if __inner_z (setq _inner_z __inner_z)) _inner_z)
  (:duration
   (&optional __duration)
   (if __duration (setq _duration __duration)) _duration)
  (:serialization-length
   ()
   (+
    ;; int16 _drone_id
    2
    ;; int32 _traj_id
    4
    ;; time _start_time
    8
    ;; uint8 _order
    1
    ;; float32[3] _start_p
    (* 4    3)
    ;; float32[3] _start_v
    (* 4    3)
    ;; float32[3] _start_a
    (* 4    3)
    ;; float32[3] _end_p
    (* 4    3)
    ;; float32[3] _end_v
    (* 4    3)
    ;; float32[3] _end_a
    (* 4    3)
    ;; float32[] _inner_x
    (* 4    (length _inner_x)) 4
    ;; float32[] _inner_y
    (* 4    (length _inner_y)) 4
    ;; float32[] _inner_z
    (* 4    (length _inner_z)) 4
    ;; float32[] _duration
    (* 4    (length _duration)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _drone_id
       (write-word _drone_id s)
     ;; int32 _traj_id
       (write-long _traj_id s)
     ;; time _start_time
       (write-long (send _start_time :sec) s) (write-long (send _start_time :nsec) s)
     ;; uint8 _order
       (write-byte _order s)
     ;; float32[3] _start_p
     (dotimes (i 3)
       (sys::poke (elt _start_p i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _start_v
     (dotimes (i 3)
       (sys::poke (elt _start_v i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _start_a
     (dotimes (i 3)
       (sys::poke (elt _start_a i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _end_p
     (dotimes (i 3)
       (sys::poke (elt _end_p i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _end_v
     (dotimes (i 3)
       (sys::poke (elt _end_v i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _end_a
     (dotimes (i 3)
       (sys::poke (elt _end_a i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _inner_x
     (write-long (length _inner_x) s)
     (dotimes (i (length _inner_x))
       (sys::poke (elt _inner_x i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _inner_y
     (write-long (length _inner_y) s)
     (dotimes (i (length _inner_y))
       (sys::poke (elt _inner_y i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _inner_z
     (write-long (length _inner_z) s)
     (dotimes (i (length _inner_z))
       (sys::poke (elt _inner_z i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _duration
     (write-long (length _duration) s)
     (dotimes (i (length _duration))
       (sys::poke (elt _duration i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _drone_id
     (setq _drone_id (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int32 _traj_id
     (setq _traj_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; time _start_time
     (send _start_time :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _start_time :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _order
     (setq _order (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32[3] _start_p
   (dotimes (i (length _start_p))
     (setf (elt _start_p i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _start_v
   (dotimes (i (length _start_v))
     (setf (elt _start_v i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _start_a
   (dotimes (i (length _start_a))
     (setf (elt _start_a i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _end_p
   (dotimes (i (length _end_p))
     (setf (elt _end_p i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _end_v
   (dotimes (i (length _end_v))
     (setf (elt _end_v i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _end_a
   (dotimes (i (length _end_a))
     (setf (elt _end_a i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[] _inner_x
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _inner_x (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _inner_x i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _inner_y
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _inner_y (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _inner_y i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _inner_z
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _inner_z (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _inner_z i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _duration
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _duration (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _duration i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get traj_utils::MINCOTraj :md5sum-) "5fcb933c545c68ff79033d88de2ca0cb")
(setf (get traj_utils::MINCOTraj :datatype-) "traj_utils/MINCOTraj")
(setf (get traj_utils::MINCOTraj :definition-)
      "int16 drone_id
int32 traj_id
time start_time

uint8 order
float32[3] start_p
float32[3] start_v
float32[3] start_a
float32[3] end_p
float32[3] end_v
float32[3] end_a
float32[] inner_x
float32[] inner_y
float32[] inner_z
float32[] duration
")



(provide :traj_utils/MINCOTraj "5fcb933c545c68ff79033d88de2ca0cb")


