; Auto-generated. Do not edit!


(cl:in-package traj_utils-msg)


;//! \htmlinclude PolyTraj.msg.html

(cl:defclass <PolyTraj> (roslisp-msg-protocol:ros-message)
  ((drone_id
    :reader drone_id
    :initarg :drone_id
    :type cl:fixnum
    :initform 0)
   (traj_id
    :reader traj_id
    :initarg :traj_id
    :type cl:integer
    :initform 0)
   (start_time
    :reader start_time
    :initarg :start_time
    :type cl:real
    :initform 0)
   (order
    :reader order
    :initarg :order
    :type cl:fixnum
    :initform 0)
   (coef_x
    :reader coef_x
    :initarg :coef_x
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (coef_y
    :reader coef_y
    :initarg :coef_y
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (coef_z
    :reader coef_z
    :initarg :coef_z
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (duration
    :reader duration
    :initarg :duration
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass PolyTraj (<PolyTraj>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PolyTraj>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PolyTraj)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name traj_utils-msg:<PolyTraj> is deprecated: use traj_utils-msg:PolyTraj instead.")))

(cl:ensure-generic-function 'drone_id-val :lambda-list '(m))
(cl:defmethod drone_id-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:drone_id-val is deprecated.  Use traj_utils-msg:drone_id instead.")
  (drone_id m))

(cl:ensure-generic-function 'traj_id-val :lambda-list '(m))
(cl:defmethod traj_id-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:traj_id-val is deprecated.  Use traj_utils-msg:traj_id instead.")
  (traj_id m))

(cl:ensure-generic-function 'start_time-val :lambda-list '(m))
(cl:defmethod start_time-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:start_time-val is deprecated.  Use traj_utils-msg:start_time instead.")
  (start_time m))

(cl:ensure-generic-function 'order-val :lambda-list '(m))
(cl:defmethod order-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:order-val is deprecated.  Use traj_utils-msg:order instead.")
  (order m))

(cl:ensure-generic-function 'coef_x-val :lambda-list '(m))
(cl:defmethod coef_x-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:coef_x-val is deprecated.  Use traj_utils-msg:coef_x instead.")
  (coef_x m))

(cl:ensure-generic-function 'coef_y-val :lambda-list '(m))
(cl:defmethod coef_y-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:coef_y-val is deprecated.  Use traj_utils-msg:coef_y instead.")
  (coef_y m))

(cl:ensure-generic-function 'coef_z-val :lambda-list '(m))
(cl:defmethod coef_z-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:coef_z-val is deprecated.  Use traj_utils-msg:coef_z instead.")
  (coef_z m))

(cl:ensure-generic-function 'duration-val :lambda-list '(m))
(cl:defmethod duration-val ((m <PolyTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:duration-val is deprecated.  Use traj_utils-msg:duration instead.")
  (duration m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PolyTraj>) ostream)
  "Serializes a message object of type '<PolyTraj>"
  (cl:let* ((signed (cl:slot-value msg 'drone_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'traj_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'start_time)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'start_time) (cl:floor (cl:slot-value msg 'start_time)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'order)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coef_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'coef_x))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coef_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'coef_y))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'coef_z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'coef_z))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'duration))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'duration))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PolyTraj>) istream)
  "Deserializes a message object of type '<PolyTraj>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'drone_id) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'traj_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'start_time) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'order)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coef_x) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coef_x)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coef_y) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coef_y)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'coef_z) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'coef_z)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'duration) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'duration)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PolyTraj>)))
  "Returns string type for a message object of type '<PolyTraj>"
  "traj_utils/PolyTraj")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PolyTraj)))
  "Returns string type for a message object of type 'PolyTraj"
  "traj_utils/PolyTraj")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PolyTraj>)))
  "Returns md5sum for a message object of type '<PolyTraj>"
  "7d47340fc475c0357eff322fb6ab494d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PolyTraj)))
  "Returns md5sum for a message object of type 'PolyTraj"
  "7d47340fc475c0357eff322fb6ab494d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PolyTraj>)))
  "Returns full string definition for message of type '<PolyTraj>"
  (cl:format cl:nil "int16 drone_id~%int32 traj_id~%time start_time~%~%uint8 order~%float32[] coef_x~%float32[] coef_y~%float32[] coef_z~%float32[] duration~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PolyTraj)))
  "Returns full string definition for message of type 'PolyTraj"
  (cl:format cl:nil "int16 drone_id~%int32 traj_id~%time start_time~%~%uint8 order~%float32[] coef_x~%float32[] coef_y~%float32[] coef_z~%float32[] duration~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PolyTraj>))
  (cl:+ 0
     2
     4
     8
     1
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coef_x) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coef_y) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'coef_z) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'duration) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PolyTraj>))
  "Converts a ROS message object to a list"
  (cl:list 'PolyTraj
    (cl:cons ':drone_id (drone_id msg))
    (cl:cons ':traj_id (traj_id msg))
    (cl:cons ':start_time (start_time msg))
    (cl:cons ':order (order msg))
    (cl:cons ':coef_x (coef_x msg))
    (cl:cons ':coef_y (coef_y msg))
    (cl:cons ':coef_z (coef_z msg))
    (cl:cons ':duration (duration msg))
))
