// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from cartographer_ros_msgs:srv/GetSlamState.idl
// generated code does not contain a copyright notice
#include "cartographer_ros_msgs/srv/detail/get_slam_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
cartographer_ros_msgs__srv__GetSlamState_Request__init(cartographer_ros_msgs__srv__GetSlamState_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Request__fini(cartographer_ros_msgs__srv__GetSlamState_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

bool
cartographer_ros_msgs__srv__GetSlamState_Request__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Request * lhs, const cartographer_ros_msgs__srv__GetSlamState_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // structure_needs_at_least_one_member
  if (lhs->structure_needs_at_least_one_member != rhs->structure_needs_at_least_one_member) {
    return false;
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Request__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Request * input,
  cartographer_ros_msgs__srv__GetSlamState_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // structure_needs_at_least_one_member
  output->structure_needs_at_least_one_member = input->structure_needs_at_least_one_member;
  return true;
}

cartographer_ros_msgs__srv__GetSlamState_Request *
cartographer_ros_msgs__srv__GetSlamState_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Request * msg = (cartographer_ros_msgs__srv__GetSlamState_Request *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cartographer_ros_msgs__srv__GetSlamState_Request));
  bool success = cartographer_ros_msgs__srv__GetSlamState_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cartographer_ros_msgs__srv__GetSlamState_Request__destroy(cartographer_ros_msgs__srv__GetSlamState_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cartographer_ros_msgs__srv__GetSlamState_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__init(cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Request * data = NULL;

  if (size) {
    data = (cartographer_ros_msgs__srv__GetSlamState_Request *)allocator.zero_allocate(size, sizeof(cartographer_ros_msgs__srv__GetSlamState_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cartographer_ros_msgs__srv__GetSlamState_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cartographer_ros_msgs__srv__GetSlamState_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__fini(cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      cartographer_ros_msgs__srv__GetSlamState_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

cartographer_ros_msgs__srv__GetSlamState_Request__Sequence *
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * array = (cartographer_ros_msgs__srv__GetSlamState_Request__Sequence *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__destroy(cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * lhs, const cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * input,
  cartographer_ros_msgs__srv__GetSlamState_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cartographer_ros_msgs__srv__GetSlamState_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cartographer_ros_msgs__srv__GetSlamState_Request * data =
      (cartographer_ros_msgs__srv__GetSlamState_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cartographer_ros_msgs__srv__GetSlamState_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cartographer_ros_msgs__srv__GetSlamState_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `status`
#include "cartographer_ros_msgs/msg/detail/status_code__functions.h"
// Member `ram_disk_path`
#include "rosidl_runtime_c/string_functions.h"

bool
cartographer_ros_msgs__srv__GetSlamState_Response__init(cartographer_ros_msgs__srv__GetSlamState_Response * msg)
{
  if (!msg) {
    return false;
  }
  // status
  if (!cartographer_ros_msgs__msg__StatusCode__init(&msg->status)) {
    cartographer_ros_msgs__srv__GetSlamState_Response__fini(msg);
    return false;
  }
  // ram_disk_path
  if (!rosidl_runtime_c__String__init(&msg->ram_disk_path)) {
    cartographer_ros_msgs__srv__GetSlamState_Response__fini(msg);
    return false;
  }
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Response__fini(cartographer_ros_msgs__srv__GetSlamState_Response * msg)
{
  if (!msg) {
    return;
  }
  // status
  cartographer_ros_msgs__msg__StatusCode__fini(&msg->status);
  // ram_disk_path
  rosidl_runtime_c__String__fini(&msg->ram_disk_path);
}

bool
cartographer_ros_msgs__srv__GetSlamState_Response__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Response * lhs, const cartographer_ros_msgs__srv__GetSlamState_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // status
  if (!cartographer_ros_msgs__msg__StatusCode__are_equal(
      &(lhs->status), &(rhs->status)))
  {
    return false;
  }
  // ram_disk_path
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->ram_disk_path), &(rhs->ram_disk_path)))
  {
    return false;
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Response__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Response * input,
  cartographer_ros_msgs__srv__GetSlamState_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // status
  if (!cartographer_ros_msgs__msg__StatusCode__copy(
      &(input->status), &(output->status)))
  {
    return false;
  }
  // ram_disk_path
  if (!rosidl_runtime_c__String__copy(
      &(input->ram_disk_path), &(output->ram_disk_path)))
  {
    return false;
  }
  return true;
}

cartographer_ros_msgs__srv__GetSlamState_Response *
cartographer_ros_msgs__srv__GetSlamState_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Response * msg = (cartographer_ros_msgs__srv__GetSlamState_Response *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cartographer_ros_msgs__srv__GetSlamState_Response));
  bool success = cartographer_ros_msgs__srv__GetSlamState_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cartographer_ros_msgs__srv__GetSlamState_Response__destroy(cartographer_ros_msgs__srv__GetSlamState_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cartographer_ros_msgs__srv__GetSlamState_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__init(cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Response * data = NULL;

  if (size) {
    data = (cartographer_ros_msgs__srv__GetSlamState_Response *)allocator.zero_allocate(size, sizeof(cartographer_ros_msgs__srv__GetSlamState_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cartographer_ros_msgs__srv__GetSlamState_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cartographer_ros_msgs__srv__GetSlamState_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__fini(cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      cartographer_ros_msgs__srv__GetSlamState_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

cartographer_ros_msgs__srv__GetSlamState_Response__Sequence *
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * array = (cartographer_ros_msgs__srv__GetSlamState_Response__Sequence *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__destroy(cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * lhs, const cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * input,
  cartographer_ros_msgs__srv__GetSlamState_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cartographer_ros_msgs__srv__GetSlamState_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cartographer_ros_msgs__srv__GetSlamState_Response * data =
      (cartographer_ros_msgs__srv__GetSlamState_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cartographer_ros_msgs__srv__GetSlamState_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cartographer_ros_msgs__srv__GetSlamState_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "cartographer_ros_msgs/srv/detail/get_slam_state__functions.h"

bool
cartographer_ros_msgs__srv__GetSlamState_Event__init(cartographer_ros_msgs__srv__GetSlamState_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    cartographer_ros_msgs__srv__GetSlamState_Event__fini(msg);
    return false;
  }
  // request
  if (!cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__init(&msg->request, 0)) {
    cartographer_ros_msgs__srv__GetSlamState_Event__fini(msg);
    return false;
  }
  // response
  if (!cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__init(&msg->response, 0)) {
    cartographer_ros_msgs__srv__GetSlamState_Event__fini(msg);
    return false;
  }
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Event__fini(cartographer_ros_msgs__srv__GetSlamState_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__fini(&msg->request);
  // response
  cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__fini(&msg->response);
}

bool
cartographer_ros_msgs__srv__GetSlamState_Event__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Event * lhs, const cartographer_ros_msgs__srv__GetSlamState_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Event__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Event * input,
  cartographer_ros_msgs__srv__GetSlamState_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!cartographer_ros_msgs__srv__GetSlamState_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!cartographer_ros_msgs__srv__GetSlamState_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

cartographer_ros_msgs__srv__GetSlamState_Event *
cartographer_ros_msgs__srv__GetSlamState_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Event * msg = (cartographer_ros_msgs__srv__GetSlamState_Event *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(cartographer_ros_msgs__srv__GetSlamState_Event));
  bool success = cartographer_ros_msgs__srv__GetSlamState_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
cartographer_ros_msgs__srv__GetSlamState_Event__destroy(cartographer_ros_msgs__srv__GetSlamState_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    cartographer_ros_msgs__srv__GetSlamState_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__init(cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Event * data = NULL;

  if (size) {
    data = (cartographer_ros_msgs__srv__GetSlamState_Event *)allocator.zero_allocate(size, sizeof(cartographer_ros_msgs__srv__GetSlamState_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = cartographer_ros_msgs__srv__GetSlamState_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        cartographer_ros_msgs__srv__GetSlamState_Event__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__fini(cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      cartographer_ros_msgs__srv__GetSlamState_Event__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

cartographer_ros_msgs__srv__GetSlamState_Event__Sequence *
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * array = (cartographer_ros_msgs__srv__GetSlamState_Event__Sequence *)allocator.allocate(sizeof(cartographer_ros_msgs__srv__GetSlamState_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__destroy(cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__are_equal(const cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * lhs, const cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
cartographer_ros_msgs__srv__GetSlamState_Event__Sequence__copy(
  const cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * input,
  cartographer_ros_msgs__srv__GetSlamState_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(cartographer_ros_msgs__srv__GetSlamState_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    cartographer_ros_msgs__srv__GetSlamState_Event * data =
      (cartographer_ros_msgs__srv__GetSlamState_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!cartographer_ros_msgs__srv__GetSlamState_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          cartographer_ros_msgs__srv__GetSlamState_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!cartographer_ros_msgs__srv__GetSlamState_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
