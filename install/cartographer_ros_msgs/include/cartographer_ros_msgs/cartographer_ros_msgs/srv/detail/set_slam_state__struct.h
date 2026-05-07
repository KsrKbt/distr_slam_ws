// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from cartographer_ros_msgs:srv/SetSlamState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "cartographer_ros_msgs/srv/set_slam_state.h"


#ifndef CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__STRUCT_H_
#define CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'ram_disk_path'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetSlamState in the package cartographer_ros_msgs.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Request
{
  /// 読み込むメモリ上のパス
  rosidl_runtime_c__String ram_disk_path;
  /// 地図を凍結するか (Pure Localizationならtrue)
  bool load_frozen_state;
} cartographer_ros_msgs__srv__SetSlamState_Request;

// Struct for a sequence of cartographer_ros_msgs__srv__SetSlamState_Request.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Request__Sequence
{
  cartographer_ros_msgs__srv__SetSlamState_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cartographer_ros_msgs__srv__SetSlamState_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'status'
#include "cartographer_ros_msgs/msg/detail/status_code__struct.h"

/// Struct defined in srv/SetSlamState in the package cartographer_ros_msgs.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Response
{
  cartographer_ros_msgs__msg__StatusCode status;
} cartographer_ros_msgs__srv__SetSlamState_Response;

// Struct for a sequence of cartographer_ros_msgs__srv__SetSlamState_Response.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Response__Sequence
{
  cartographer_ros_msgs__srv__SetSlamState_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cartographer_ros_msgs__srv__SetSlamState_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  cartographer_ros_msgs__srv__SetSlamState_Event__request__MAX_SIZE = 1
};
// response
enum
{
  cartographer_ros_msgs__srv__SetSlamState_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/SetSlamState in the package cartographer_ros_msgs.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Event
{
  service_msgs__msg__ServiceEventInfo info;
  cartographer_ros_msgs__srv__SetSlamState_Request__Sequence request;
  cartographer_ros_msgs__srv__SetSlamState_Response__Sequence response;
} cartographer_ros_msgs__srv__SetSlamState_Event;

// Struct for a sequence of cartographer_ros_msgs__srv__SetSlamState_Event.
typedef struct cartographer_ros_msgs__srv__SetSlamState_Event__Sequence
{
  cartographer_ros_msgs__srv__SetSlamState_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} cartographer_ros_msgs__srv__SetSlamState_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__STRUCT_H_
