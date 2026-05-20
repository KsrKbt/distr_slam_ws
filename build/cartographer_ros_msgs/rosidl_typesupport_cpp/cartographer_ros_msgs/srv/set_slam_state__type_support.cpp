// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from cartographer_ros_msgs:srv/SetSlamState.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "cartographer_ros_msgs/srv/detail/set_slam_state__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace cartographer_ros_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetSlamState_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetSlamState_Request_type_support_ids_t;

static const _SetSlamState_Request_type_support_ids_t _SetSlamState_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetSlamState_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetSlamState_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetSlamState_Request_type_support_symbol_names_t _SetSlamState_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, SetSlamState_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, SetSlamState_Request)),
  }
};

typedef struct _SetSlamState_Request_type_support_data_t
{
  void * data[2];
} _SetSlamState_Request_type_support_data_t;

static _SetSlamState_Request_type_support_data_t _SetSlamState_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetSlamState_Request_message_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_SetSlamState_Request_message_typesupport_ids.typesupport_identifier[0],
  &_SetSlamState_Request_message_typesupport_symbol_names.symbol_name[0],
  &_SetSlamState_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SetSlamState_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetSlamState_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cartographer_ros_msgs::srv::SetSlamState_Request>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::SetSlamState_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, SetSlamState_Request)() {
  return get_message_type_support_handle<cartographer_ros_msgs::srv::SetSlamState_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "cartographer_ros_msgs/srv/detail/set_slam_state__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace cartographer_ros_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetSlamState_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetSlamState_Response_type_support_ids_t;

static const _SetSlamState_Response_type_support_ids_t _SetSlamState_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetSlamState_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetSlamState_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetSlamState_Response_type_support_symbol_names_t _SetSlamState_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, SetSlamState_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, SetSlamState_Response)),
  }
};

typedef struct _SetSlamState_Response_type_support_data_t
{
  void * data[2];
} _SetSlamState_Response_type_support_data_t;

static _SetSlamState_Response_type_support_data_t _SetSlamState_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetSlamState_Response_message_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_SetSlamState_Response_message_typesupport_ids.typesupport_identifier[0],
  &_SetSlamState_Response_message_typesupport_symbol_names.symbol_name[0],
  &_SetSlamState_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SetSlamState_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetSlamState_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cartographer_ros_msgs::srv::SetSlamState_Response>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::SetSlamState_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, SetSlamState_Response)() {
  return get_message_type_support_handle<cartographer_ros_msgs::srv::SetSlamState_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "cartographer_ros_msgs/srv/detail/set_slam_state__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace cartographer_ros_msgs
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetSlamState_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetSlamState_type_support_ids_t;

static const _SetSlamState_type_support_ids_t _SetSlamState_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetSlamState_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetSlamState_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetSlamState_type_support_symbol_names_t _SetSlamState_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, SetSlamState)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, SetSlamState)),
  }
};

typedef struct _SetSlamState_type_support_data_t
{
  void * data[2];
} _SetSlamState_type_support_data_t;

static _SetSlamState_type_support_data_t _SetSlamState_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetSlamState_service_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_SetSlamState_service_typesupport_ids.typesupport_identifier[0],
  &_SetSlamState_service_typesupport_symbol_names.symbol_name[0],
  &_SetSlamState_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t SetSlamState_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetSlamState_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<cartographer_ros_msgs::srv::SetSlamState>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::SetSlamState_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, SetSlamState)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<cartographer_ros_msgs::srv::SetSlamState>();
}

#ifdef __cplusplus
}
#endif
