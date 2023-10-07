// Auto-generated. Do not edit!

// (in-package traj_utils.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class PolyTraj {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.drone_id = null;
      this.traj_id = null;
      this.start_time = null;
      this.order = null;
      this.coef_x = null;
      this.coef_y = null;
      this.coef_z = null;
      this.duration = null;
    }
    else {
      if (initObj.hasOwnProperty('drone_id')) {
        this.drone_id = initObj.drone_id
      }
      else {
        this.drone_id = 0;
      }
      if (initObj.hasOwnProperty('traj_id')) {
        this.traj_id = initObj.traj_id
      }
      else {
        this.traj_id = 0;
      }
      if (initObj.hasOwnProperty('start_time')) {
        this.start_time = initObj.start_time
      }
      else {
        this.start_time = {secs: 0, nsecs: 0};
      }
      if (initObj.hasOwnProperty('order')) {
        this.order = initObj.order
      }
      else {
        this.order = 0;
      }
      if (initObj.hasOwnProperty('coef_x')) {
        this.coef_x = initObj.coef_x
      }
      else {
        this.coef_x = [];
      }
      if (initObj.hasOwnProperty('coef_y')) {
        this.coef_y = initObj.coef_y
      }
      else {
        this.coef_y = [];
      }
      if (initObj.hasOwnProperty('coef_z')) {
        this.coef_z = initObj.coef_z
      }
      else {
        this.coef_z = [];
      }
      if (initObj.hasOwnProperty('duration')) {
        this.duration = initObj.duration
      }
      else {
        this.duration = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PolyTraj
    // Serialize message field [drone_id]
    bufferOffset = _serializer.int16(obj.drone_id, buffer, bufferOffset);
    // Serialize message field [traj_id]
    bufferOffset = _serializer.int32(obj.traj_id, buffer, bufferOffset);
    // Serialize message field [start_time]
    bufferOffset = _serializer.time(obj.start_time, buffer, bufferOffset);
    // Serialize message field [order]
    bufferOffset = _serializer.uint8(obj.order, buffer, bufferOffset);
    // Serialize message field [coef_x]
    bufferOffset = _arraySerializer.float32(obj.coef_x, buffer, bufferOffset, null);
    // Serialize message field [coef_y]
    bufferOffset = _arraySerializer.float32(obj.coef_y, buffer, bufferOffset, null);
    // Serialize message field [coef_z]
    bufferOffset = _arraySerializer.float32(obj.coef_z, buffer, bufferOffset, null);
    // Serialize message field [duration]
    bufferOffset = _arraySerializer.float32(obj.duration, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PolyTraj
    let len;
    let data = new PolyTraj(null);
    // Deserialize message field [drone_id]
    data.drone_id = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [traj_id]
    data.traj_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [start_time]
    data.start_time = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [order]
    data.order = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [coef_x]
    data.coef_x = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [coef_y]
    data.coef_y = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [coef_z]
    data.coef_z = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [duration]
    data.duration = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.coef_x.length;
    length += 4 * object.coef_y.length;
    length += 4 * object.coef_z.length;
    length += 4 * object.duration.length;
    return length + 31;
  }

  static datatype() {
    // Returns string type for a message object
    return 'traj_utils/PolyTraj';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7d47340fc475c0357eff322fb6ab494d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int16 drone_id
    int32 traj_id
    time start_time
    
    uint8 order
    float32[] coef_x
    float32[] coef_y
    float32[] coef_z
    float32[] duration
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PolyTraj(null);
    if (msg.drone_id !== undefined) {
      resolved.drone_id = msg.drone_id;
    }
    else {
      resolved.drone_id = 0
    }

    if (msg.traj_id !== undefined) {
      resolved.traj_id = msg.traj_id;
    }
    else {
      resolved.traj_id = 0
    }

    if (msg.start_time !== undefined) {
      resolved.start_time = msg.start_time;
    }
    else {
      resolved.start_time = {secs: 0, nsecs: 0}
    }

    if (msg.order !== undefined) {
      resolved.order = msg.order;
    }
    else {
      resolved.order = 0
    }

    if (msg.coef_x !== undefined) {
      resolved.coef_x = msg.coef_x;
    }
    else {
      resolved.coef_x = []
    }

    if (msg.coef_y !== undefined) {
      resolved.coef_y = msg.coef_y;
    }
    else {
      resolved.coef_y = []
    }

    if (msg.coef_z !== undefined) {
      resolved.coef_z = msg.coef_z;
    }
    else {
      resolved.coef_z = []
    }

    if (msg.duration !== undefined) {
      resolved.duration = msg.duration;
    }
    else {
      resolved.duration = []
    }

    return resolved;
    }
};

module.exports = PolyTraj;
