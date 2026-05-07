// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from cartographer_ros_msgs:srv/GetSlamState.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "cartographer_ros_msgs/srv/detail/get_slam_state__functions.h"
#include "cartographer_ros_msgs/srv/detail/get_slam_state__struct.hpp"
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

typedef struct _GetSlamState_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _GetSlamState_Request_type_support_ids_t;

static const _GetSlamState_Request_type_support_ids_t _GetSlamState_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _GetSlamState_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _GetSlamState_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _GetSlamState_Request_type_support_symbol_names_t _GetSlamState_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, GetSlamState_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, GetSlamState_Request)),
  }
};

typedef struct _GetSlamState_Request_type_support_data_t
{
  void * data[2];
} _GetSlamState_Request_type_support_data_t;

static _GetSlamState_Request_type_support_data_t _GetSlamState_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _GetSlamState_Request_message_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_GetSlamState_Request_message_typesupport_ids.typesupport_identifier[0],
  &_GetSlamState_Request_message_typesupport_symbol_names.symbol_name[0],
  &_GetSlamState_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t GetSlamState_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_GetSlamState_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &cartographer_ros_msgs__srv__GetSlamState_Request__get_type_hash,
  &cartographer_ros_msgs__srv__GetSlamState_Request__get_type_description,
  &cartographer_ros_msgs__srv__GetSlamState_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Request>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::GetSlamState_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, GetSlamState_Request)() {
  return get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Request>();
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
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__functions.h"
// already included above
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__struct.hpp"
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

typedef struct _GetSlamState_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _GetSlamState_Response_type_support_ids_t;

static const _GetSlamState_Response_type_support_ids_t _GetSlamState_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _GetSlamState_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _GetSlamState_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _GetSlamState_Response_type_support_symbol_names_t _GetSlamState_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, GetSlamState_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, GetSlamState_Response)),
  }
};

typedef struct _GetSlamState_Response_type_support_data_t
{
  void * data[2];
} _GetSlamState_Response_type_support_data_t;

static _GetSlamState_Response_type_support_data_t _GetSlamState_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _GetSlamState_Response_message_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_GetSlamState_Response_message_typesupport_ids.typesupport_identifier[0],
  &_GetSlamState_Response_message_typesupport_symbol_names.symbol_name[0],
  &_GetSlamState_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t GetSlamState_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_GetSlamState_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &cartographer_ros_msgs__srv__GetSlamState_Response__get_type_hash,
  &cartographer_ros_msgs__srv__GetSlamState_Response__get_type_description,
  &cartographer_ros_msgs__srv__GetSlamState_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Response>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::GetSlamState_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, GetSlamState_Response)() {
  return get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Response>();
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
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__functions.h"
// already included above
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__struct.hpp"
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

typedef struct _GetSlamState_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _GetSlamState_Event_type_support_ids_t;

static const _GetSlamState_Event_type_support_ids_t _GetSlamState_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _GetSlamState_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _GetSlamState_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _GetSlamState_Event_type_support_symbol_names_t _GetSlamState_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, GetSlamState_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, GetSlamState_Event)),
  }
};

typedef struct _GetSlamState_Event_type_support_data_t
{
  void * data[2];
} _GetSlamState_Event_type_support_data_t;

static _GetSlamState_Event_type_support_data_t _GetSlamState_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _GetSlamState_Event_message_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_GetSlamState_Event_message_typesupport_ids.typesupport_identifier[0],
  &_GetSlamState_Event_message_typesupport_symbol_names.symbol_name[0],
  &_GetSlamState_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t GetSlamState_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_GetSlamState_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &cartographer_ros_msgs__srv__GetSlamState_Event__get_type_hash,
  &cartographer_ros_msgs__srv__GetSlamState_Event__get_type_description,
  &cartographer_ros_msgs__srv__GetSlamState_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Event>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::GetSlamState_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, GetSlamState_Event)() {
  return get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
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

typedef struct _GetSlamState_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _GetSlamState_type_support_ids_t;

static const _GetSlamState_type_support_ids_t _GetSlamState_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _GetSlamState_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _GetSlamState_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _GetSlamState_type_support_symbol_names_t _GetSlamState_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, cartographer_ros_msgs, srv, GetSlamState)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, cartographer_ros_msgs, srv, GetSlamState)),
  }
};

typedef struct _GetSlamState_type_support_data_t
{
  void * data[2];
} _GetSlamState_type_support_data_t;

static _GetSlamState_type_support_data_t _GetSlamState_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _GetSlamState_service_typesupport_map = {
  2,
  "cartographer_ros_msgs",
  &_GetSlamState_service_typesupport_ids.typesupport_identifier[0],
  &_GetSlamState_service_typesupport_symbol_names.symbol_name[0],
  &_GetSlamState_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t GetSlamState_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_GetSlamState_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<cartographer_ros_msgs::srv::GetSlamState_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<cartographer_ros_msgs::srv::GetSlamState>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<cartographer_ros_msgs::srv::GetSlamState>,
  &cartographer_ros_msgs__srv__GetSlamState__get_type_hash,
  &cartographer_ros_msgs__srv__GetSlamState__get_type_description,
  &cartographer_ros_msgs__srv__GetSlamState__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<cartographer_ros_msgs::srv::GetSlamState>()
{
  return &::cartographer_ros_msgs::srv::rosidl_typesupport_cpp::GetSlamState_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, cartographer_ros_msgs, srv, GetSlamState)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<cartographer_ros_msgs::srv::GetSlamState>();
}

#ifdef __cplusplus
}
#endif
