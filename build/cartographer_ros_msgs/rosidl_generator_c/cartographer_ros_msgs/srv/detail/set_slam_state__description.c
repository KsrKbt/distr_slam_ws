// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from cartographer_ros_msgs:srv/SetSlamState.idl
// generated code does not contain a copyright notice

#include "cartographer_ros_msgs/srv/detail/set_slam_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_cartographer_ros_msgs
const rosidl_type_hash_t *
cartographer_ros_msgs__srv__SetSlamState__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x67, 0xf0, 0x62, 0x34, 0x90, 0x7f, 0x55, 0xc6,
      0xd5, 0xfc, 0x3a, 0x38, 0x1f, 0xcc, 0x3a, 0x05,
      0x05, 0xf7, 0xc9, 0x97, 0x5f, 0xf5, 0x1b, 0xe2,
      0xb6, 0x62, 0x1a, 0x64, 0x26, 0x03, 0x68, 0x79,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_cartographer_ros_msgs
const rosidl_type_hash_t *
cartographer_ros_msgs__srv__SetSlamState_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x50, 0x2e, 0xb5, 0x09, 0x05, 0x31, 0x95, 0xe1,
      0xa8, 0x2d, 0x90, 0x23, 0x49, 0xa8, 0x90, 0x55,
      0x52, 0x4e, 0x29, 0x90, 0x4d, 0xc4, 0x4f, 0x6e,
      0x7a, 0x00, 0x26, 0x6b, 0x82, 0xe8, 0xf3, 0xbd,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_cartographer_ros_msgs
const rosidl_type_hash_t *
cartographer_ros_msgs__srv__SetSlamState_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb4, 0xf6, 0x10, 0x27, 0xa2, 0x01, 0x57, 0xbf,
      0x88, 0x48, 0x8a, 0xf6, 0xf9, 0x49, 0x52, 0x2e,
      0xcc, 0xf4, 0x99, 0xe6, 0x1b, 0xd3, 0x2f, 0x66,
      0xd5, 0xb1, 0xf8, 0x37, 0x51, 0xe1, 0x2a, 0xa5,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_cartographer_ros_msgs
const rosidl_type_hash_t *
cartographer_ros_msgs__srv__SetSlamState_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd8, 0x1e, 0xe3, 0xcb, 0xe7, 0x22, 0xd1, 0x2f,
      0x47, 0x24, 0xcc, 0xf6, 0xf9, 0xcc, 0x69, 0x5e,
      0x7e, 0x18, 0xd0, 0x05, 0x57, 0x7c, 0xa6, 0x9f,
      0xda, 0x25, 0xd4, 0x54, 0x49, 0x7b, 0xc0, 0x9e,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "cartographer_ros_msgs/msg/detail/status_code__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t cartographer_ros_msgs__msg__StatusCode__EXPECTED_HASH = {1, {
    0xc9, 0x40, 0x95, 0x39, 0xf9, 0x2c, 0x15, 0x7f,
    0x90, 0x23, 0xed, 0x72, 0xff, 0x50, 0xf8, 0xcb,
    0xef, 0xd2, 0x2a, 0x89, 0x1f, 0xb2, 0x40, 0x8b,
    0x51, 0x41, 0xcc, 0xc3, 0x5e, 0x35, 0xfa, 0xad,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char cartographer_ros_msgs__srv__SetSlamState__TYPE_NAME[] = "cartographer_ros_msgs/srv/SetSlamState";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char cartographer_ros_msgs__msg__StatusCode__TYPE_NAME[] = "cartographer_ros_msgs/msg/StatusCode";
static char cartographer_ros_msgs__srv__SetSlamState_Event__TYPE_NAME[] = "cartographer_ros_msgs/srv/SetSlamState_Event";
static char cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME[] = "cartographer_ros_msgs/srv/SetSlamState_Request";
static char cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME[] = "cartographer_ros_msgs/srv/SetSlamState_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__request_message[] = "request_message";
static char cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__response_message[] = "response_message";
static char cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field cartographer_ros_msgs__srv__SetSlamState__FIELDS[] = {
  {
    {cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
    },
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
    },
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {cartographer_ros_msgs__srv__SetSlamState_Event__TYPE_NAME, 44, 44},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription cartographer_ros_msgs__srv__SetSlamState__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__msg__StatusCode__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Event__TYPE_NAME, 44, 44},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
cartographer_ros_msgs__srv__SetSlamState__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {cartographer_ros_msgs__srv__SetSlamState__TYPE_NAME, 38, 38},
      {cartographer_ros_msgs__srv__SetSlamState__FIELDS, 3, 3},
    },
    {cartographer_ros_msgs__srv__SetSlamState__REFERENCED_TYPE_DESCRIPTIONS, 6, 6},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&cartographer_ros_msgs__msg__StatusCode__EXPECTED_HASH, cartographer_ros_msgs__msg__StatusCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = cartographer_ros_msgs__msg__StatusCode__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = cartographer_ros_msgs__srv__SetSlamState_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = cartographer_ros_msgs__srv__SetSlamState_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[4].fields = cartographer_ros_msgs__srv__SetSlamState_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char cartographer_ros_msgs__srv__SetSlamState_Request__FIELD_NAME__ram_disk_path[] = "ram_disk_path";
static char cartographer_ros_msgs__srv__SetSlamState_Request__FIELD_NAME__load_frozen_state[] = "load_frozen_state";

static rosidl_runtime_c__type_description__Field cartographer_ros_msgs__srv__SetSlamState_Request__FIELDS[] = {
  {
    {cartographer_ros_msgs__srv__SetSlamState_Request__FIELD_NAME__ram_disk_path, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Request__FIELD_NAME__load_frozen_state, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
cartographer_ros_msgs__srv__SetSlamState_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
      {cartographer_ros_msgs__srv__SetSlamState_Request__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char cartographer_ros_msgs__srv__SetSlamState_Response__FIELD_NAME__status[] = "status";

static rosidl_runtime_c__type_description__Field cartographer_ros_msgs__srv__SetSlamState_Response__FIELDS[] = {
  {
    {cartographer_ros_msgs__srv__SetSlamState_Response__FIELD_NAME__status, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {cartographer_ros_msgs__msg__StatusCode__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription cartographer_ros_msgs__srv__SetSlamState_Response__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {cartographer_ros_msgs__msg__StatusCode__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
cartographer_ros_msgs__srv__SetSlamState_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
      {cartographer_ros_msgs__srv__SetSlamState_Response__FIELDS, 1, 1},
    },
    {cartographer_ros_msgs__srv__SetSlamState_Response__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&cartographer_ros_msgs__msg__StatusCode__EXPECTED_HASH, cartographer_ros_msgs__msg__StatusCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = cartographer_ros_msgs__msg__StatusCode__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__info[] = "info";
static char cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__request[] = "request";
static char cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field cartographer_ros_msgs__srv__SetSlamState_Event__FIELDS[] = {
  {
    {cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
    },
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription cartographer_ros_msgs__srv__SetSlamState_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__msg__StatusCode__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
    {NULL, 0, 0},
  },
  {
    {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
cartographer_ros_msgs__srv__SetSlamState_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {cartographer_ros_msgs__srv__SetSlamState_Event__TYPE_NAME, 44, 44},
      {cartographer_ros_msgs__srv__SetSlamState_Event__FIELDS, 3, 3},
    },
    {cartographer_ros_msgs__srv__SetSlamState_Event__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&cartographer_ros_msgs__msg__StatusCode__EXPECTED_HASH, cartographer_ros_msgs__msg__StatusCode__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = cartographer_ros_msgs__msg__StatusCode__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = cartographer_ros_msgs__srv__SetSlamState_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = cartographer_ros_msgs__srv__SetSlamState_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Request\n"
  "string ram_disk_path       # \\xe8\\xaa\\xad\\xe3\\x81\\xbf\\xe8\\xbe\\xbc\\xe3\\x82\\x80\\xe3\\x83\\xa1\\xe3\\x83\\xa2\\xe3\\x83\\xaa\\xe4\\xb8\\x8a\\xe3\\x81\\xae\\xe3\\x83\\x91\\xe3\\x82\\xb9\n"
  "bool load_frozen_state     # \\xe5\\x9c\\xb0\\xe5\\x9b\\xb3\\xe3\\x82\\x92\\xe5\\x87\\x8d\\xe7\\xb5\\x90\\xe3\\x81\\x99\\xe3\\x82\\x8b\\xe3\\x81\\x8b (Pure Localization\\xe3\\x81\\xaa\\xe3\\x82\\x89true)\n"
  "---\n"
  "# Response\n"
  "cartographer_ros_msgs/StatusCode status";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
cartographer_ros_msgs__srv__SetSlamState__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {cartographer_ros_msgs__srv__SetSlamState__TYPE_NAME, 38, 38},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 169, 169},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
cartographer_ros_msgs__srv__SetSlamState_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {cartographer_ros_msgs__srv__SetSlamState_Request__TYPE_NAME, 46, 46},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
cartographer_ros_msgs__srv__SetSlamState_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {cartographer_ros_msgs__srv__SetSlamState_Response__TYPE_NAME, 47, 47},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
cartographer_ros_msgs__srv__SetSlamState_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {cartographer_ros_msgs__srv__SetSlamState_Event__TYPE_NAME, 44, 44},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
cartographer_ros_msgs__srv__SetSlamState__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[7];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 7, 7};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *cartographer_ros_msgs__srv__SetSlamState__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *cartographer_ros_msgs__msg__StatusCode__get_individual_type_description_source(NULL);
    sources[3] = *cartographer_ros_msgs__srv__SetSlamState_Event__get_individual_type_description_source(NULL);
    sources[4] = *cartographer_ros_msgs__srv__SetSlamState_Request__get_individual_type_description_source(NULL);
    sources[5] = *cartographer_ros_msgs__srv__SetSlamState_Response__get_individual_type_description_source(NULL);
    sources[6] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
cartographer_ros_msgs__srv__SetSlamState_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *cartographer_ros_msgs__srv__SetSlamState_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
cartographer_ros_msgs__srv__SetSlamState_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *cartographer_ros_msgs__srv__SetSlamState_Response__get_individual_type_description_source(NULL),
    sources[1] = *cartographer_ros_msgs__msg__StatusCode__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
cartographer_ros_msgs__srv__SetSlamState_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *cartographer_ros_msgs__srv__SetSlamState_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *cartographer_ros_msgs__msg__StatusCode__get_individual_type_description_source(NULL);
    sources[3] = *cartographer_ros_msgs__srv__SetSlamState_Request__get_individual_type_description_source(NULL);
    sources[4] = *cartographer_ros_msgs__srv__SetSlamState_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
