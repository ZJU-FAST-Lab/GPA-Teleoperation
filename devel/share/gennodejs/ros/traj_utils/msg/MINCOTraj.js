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

class MINCOTraj {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.drone_id = null;
      this.traj_id = null;
      this.start_time = null;
      this.order = null;
      this.start_p = null;
      this.start_v = null;
      this.start_a = null;
      this.end_p = null;
      this.end_v = null;
      this.end_a = null;
      this.inner_x = null;
      this.inner_y = null;
      this.inner_z = null;
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
      if (initObj.hasOwnProperty('start_p')) {
        this.start_p = initObj.start_p
      }
      else {
        this.start_p = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('start_v')) {
        this.start_v = initObj.start_v
      }
      else {
        this.start_v = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('start_a')) {
        this.start_a = initObj.start_a
      }
      else {
        this.start_a = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('end_p')) {
        this.end_p = initObj.end_p
      }
      else {
        this.end_p = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('end_v')) {
        this.end_v = initObj.end_v
      }
      else {
        this.end_v = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('end_a')) {
        this.end_a = initObj.end_a
      }
      else {
        this.end_a = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('inner_x')) {
        this.inner_x = initObj.inner_x
      }
      else {
        this.inner_x = [];
      }
      if (initObj.hasOwnProperty('inner_y')) {
        this.inner_y = initObj.inner_y
      }
      else {
        this.inner_y = [];
      }
      if (initObj.hasOwnProperty('inner_z')) {
        this.inner_z = initObj.inner_z
      }
      else {
        this.inner_z = [];
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
    // Serializes a message object of type MINCOTraj
    // Serialize message field [drone_id]
    bufferOffset = _serializer.int16(obj.drone_id, buffer, bufferOffset);
    // Serialize message field [traj_id]
    bufferOffset = _serializer.int32(obj.traj_id, buffer, bufferOffset);
    // Serialize message field [start_time]
    bufferOffset = _serializer.time(obj.start_time, buffer, bufferOffset);
    // Serialize message field [order]
    bufferOffset = _serializer.uint8(obj.order, buffer, bufferOffset);
    // Check that the constant length array field [start_p] has the right length
    if (obj.start_p.length !== 3) {
      throw new Error('Unable to serialize array field start_p - length must be 3')
    }
    // Serialize message field [start_p]
    bufferOffset = _arraySerializer.float32(obj.start_p, buffer, bufferOffset, 3);
    // Check that the constant length array field [start_v] has the right length
    if (obj.start_v.length !== 3) {
      throw new Error('Unable to serialize array field start_v - length must be 3')
    }
    // Serialize message field [start_v]
    bufferOffset = _arraySerializer.float32(obj.start_v, buffer, bufferOffset, 3);
    // Check that the constant length array field [start_a] has the right length
    if (obj.start_a.length !== 3) {
      throw new Error('Unable to serialize array field start_a - length must be 3')
    }
    // Serialize message field [start_a]
    bufferOffset = _arraySerializer.float32(obj.start_a, buffer, bufferOffset, 3);
    // Check that the constant length array field [end_p] has the right length
    if (obj.end_p.length !== 3) {
      throw new Error('Unable to serialize array field end_p - length must be 3')
    }
    // Serialize message field [end_p]
    bufferOffset = _arraySerializer.float32(obj.end_p, buffer, bufferOffset, 3);
    // Check that the constant length array field [end_v] has the right length
    if (obj.end_v.length !== 3) {
      throw new Error('Unable to serialize array field end_v - length must be 3')
    }
    // Serialize message field [end_v]
    bufferOffset = _arraySerializer.float32(obj.end_v, buffer, bufferOffset, 3);
    // Check that the constant length array field [end_a] has the right length
    if (obj.end_a.length !== 3) {
      throw new Error('Unable to serialize array field end_a - length must be 3')
    }
    // Serialize message field [end_a]
    bufferOffset = _arraySerializer.float32(obj.end_a, buffer, bufferOffset, 3);
    // Serialize message field [inner_x]
    bufferOffset = _arraySerializer.float32(obj.inner_x, buffer, bufferOffset, null);
    // Serialize message field [inner_y]
    bufferOffset = _arraySerializer.float32(obj.inner_y, buffer, bufferOffset, null);
    // Serialize message field [inner_z]
    bufferOffset = _arraySerializer.float32(obj.inner_z, buffer, bufferOffset, null);
    // Serialize message field [duration]
    bufferOffset = _arraySerializer.float32(obj.duration, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type MINCOTraj
    let len;
    let data = new MINCOTraj(null);
    // Deserialize message field [drone_id]
    data.drone_id = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [traj_id]
    data.traj_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [start_time]
    data.start_time = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [order]
    data.order = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [start_p]
    data.start_p = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [start_v]
    data.start_v = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [start_a]
    data.start_a = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [end_p]
    data.end_p = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [end_v]
    data.end_v = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [end_a]
    data.end_a = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [inner_x]
    data.inner_x = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [inner_y]
    data.inner_y = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [inner_z]
    data.inner_z = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [duration]
    data.duration = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.inner_x.length;
    length += 4 * object.inner_y.length;
    length += 4 * object.inner_z.length;
    length += 4 * object.duration.length;
    return length + 103;
  }

  static datatype() {
    // Returns string type for a message object
    return 'traj_utils/MINCOTraj';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5fcb933c545c68ff79033d88de2ca0cb';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int16 drone_id
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
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new MINCOTraj(null);
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

    if (msg.start_p !== undefined) {
      resolved.start_p = msg.start_p;
    }
    else {
      resolved.start_p = new Array(3).fill(0)
    }

    if (msg.start_v !== undefined) {
      resolved.start_v = msg.start_v;
    }
    else {
      resolved.start_v = new Array(3).fill(0)
    }

    if (msg.start_a !== undefined) {
      resolved.start_a = msg.start_a;
    }
    else {
      resolved.start_a = new Array(3).fill(0)
    }

    if (msg.end_p !== undefined) {
      resolved.end_p = msg.end_p;
    }
    else {
      resolved.end_p = new Array(3).fill(0)
    }

    if (msg.end_v !== undefined) {
      resolved.end_v = msg.end_v;
    }
    else {
      resolved.end_v = new Array(3).fill(0)
    }

    if (msg.end_a !== undefined) {
      resolved.end_a = msg.end_a;
    }
    else {
      resolved.end_a = new Array(3).fill(0)
    }

    if (msg.inner_x !== undefined) {
      resolved.inner_x = msg.inner_x;
    }
    else {
      resolved.inner_x = []
    }

    if (msg.inner_y !== undefined) {
      resolved.inner_y = msg.inner_y;
    }
    else {
      resolved.inner_y = []
    }

    if (msg.inner_z !== undefined) {
      resolved.inner_z = msg.inner_z;
    }
    else {
      resolved.inner_z = []
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

module.exports = MINCOTraj;
