;; Auto-generated. Do not edit!


(when (boundp 'traj_utils::PolyTraj)
  (if (not (find-package "TRAJ_UTILS"))
    (make-package "TRAJ_UTILS"))
  (shadow 'PolyTraj (find-package "TRAJ_UTILS")))
(unless (find-package "TRAJ_UTILS::POLYTRAJ")
  (make-package "TRAJ_UTILS::POLYTRAJ"))

(in-package "ROS")
;;//! \htmlinclude PolyTraj.msg.html


(defclass traj_utils::PolyTraj
  :super ros::object
  :slots (_drone_id _traj_id _start_time _order _coef_x _coef_y _coef_z _duration ))

(defmethod traj_utils::PolyTraj
  (:init
   (&key
    ((:drone_id __drone_id) 0)
    ((:traj_id __traj_id) 0)
    ((:start_time __start_time) (instance ros::time :init))
    ((:order __order) 0)
    ((:coef_x __coef_x) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:coef_y __coef_y) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:coef_z __coef_z) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:duration __duration) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _drone_id (round __drone_id))
   (setq _traj_id (round __traj_id))
   (setq _start_time __start_time)
   (setq _order (round __order))
   (setq _coef_x __coef_x)
   (setq _coef_y __coef_y)
   (setq _coef_z __coef_z)
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
  (:coef_x
   (&optional __coef_x)
   (if __coef_x (setq _coef_x __coef_x)) _coef_x)
  (:coef_y
   (&optional __coef_y)
   (if __coef_y (setq _coef_y __coef_y)) _coef_y)
  (:coef_z
   (&optional __coef_z)
   (if __coef_z (setq _coef_z __coef_z)) _coef_z)
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
    ;; float32[] _coef_x
    (* 4    (length _coef_x)) 4
    ;; float32[] _coef_y
    (* 4    (length _coef_y)) 4
    ;; float32[] _coef_z
    (* 4    (length _coef_z)) 4
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
     ;; float32[] _coef_x
     (write-long (length _coef_x) s)
     (dotimes (i (length _coef_x))
       (sys::poke (elt _coef_x i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _coef_y
     (write-long (length _coef_y) s)
     (dotimes (i (length _coef_y))
       (sys::poke (elt _coef_y i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _coef_z
     (write-long (length _coef_z) s)
     (dotimes (i (length _coef_z))
       (sys::poke (elt _coef_z i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; float32[] _coef_x
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coef_x (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _coef_x i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _coef_y
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coef_y (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _coef_y i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _coef_z
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _coef_z (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _coef_z i) (sys::peek buf ptr- :float)) (incf ptr- 4)
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

(setf (get traj_utils::PolyTraj :md5sum-) "7d47340fc475c0357eff322fb6ab494d")
(setf (get traj_utils::PolyTraj :datatype-) "traj_utils/PolyTraj")
(setf (get traj_utils::PolyTraj :definition-)
      "int16 drone_id
int32 traj_id
time start_time

uint8 order
float32[] coef_x
float32[] coef_y
float32[] coef_z
float32[] duration

")



(provide :traj_utils/PolyTraj "7d47340fc475c0357eff322fb6ab494d")


