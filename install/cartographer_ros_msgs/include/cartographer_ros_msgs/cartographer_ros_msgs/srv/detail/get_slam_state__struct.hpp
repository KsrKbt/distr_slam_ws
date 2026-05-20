// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from cartographer_ros_msgs:srv/GetSlamState.idl
// generated code does not contain a copyright notice

#ifndef CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__GET_SLAM_STATE__STRUCT_HPP_
#define CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__GET_SLAM_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Request __attribute__((deprecated))
#else
# define DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Request __declspec(deprecated)
#endif

namespace cartographer_ros_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSlamState_Request_
{
  using Type = GetSlamState_Request_<ContainerAllocator>;

  explicit GetSlamState_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetSlamState_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Request
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Request
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSlamState_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSlamState_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSlamState_Request_

// alias to use template instance with default allocator
using GetSlamState_Request =
  cartographer_ros_msgs::srv::GetSlamState_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace cartographer_ros_msgs


// Include directives for member types
// Member 'status'
#include "cartographer_ros_msgs/msg/detail/status_code__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Response __attribute__((deprecated))
#else
# define DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Response __declspec(deprecated)
#endif

namespace cartographer_ros_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSlamState_Response_
{
  using Type = GetSlamState_Response_<ContainerAllocator>;

  explicit GetSlamState_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : status(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ram_disk_path = "";
    }
  }

  explicit GetSlamState_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : status(_alloc, _init),
    ram_disk_path(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ram_disk_path = "";
    }
  }

  // field types and members
  using _status_type =
    cartographer_ros_msgs::msg::StatusCode_<ContainerAllocator>;
  _status_type status;
  using _ram_disk_path_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _ram_disk_path_type ram_disk_path;

  // setters for named parameter idiom
  Type & set__status(
    const cartographer_ros_msgs::msg::StatusCode_<ContainerAllocator> & _arg)
  {
    this->status = _arg;
    return *this;
  }
  Type & set__ram_disk_path(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->ram_disk_path = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Response
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__cartographer_ros_msgs__srv__GetSlamState_Response
    std::shared_ptr<cartographer_ros_msgs::srv::GetSlamState_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSlamState_Response_ & other) const
  {
    if (this->status != other.status) {
      return false;
    }
    if (this->ram_disk_path != other.ram_disk_path) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSlamState_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSlamState_Response_

// alias to use template instance with default allocator
using GetSlamState_Response =
  cartographer_ros_msgs::srv::GetSlamState_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace cartographer_ros_msgs

namespace cartographer_ros_msgs
{

namespace srv
{

struct GetSlamState
{
  using Request = cartographer_ros_msgs::srv::GetSlamState_Request;
  using Response = cartographer_ros_msgs::srv::GetSlamState_Response;
};

}  // namespace srv

}  // namespace cartographer_ros_msgs

#endif  // CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__GET_SLAM_STATE__STRUCT_HPP_
