# generated from rosidl_generator_py/resource/_idl.py.em
# with input from cartographer_ros_msgs:srv/SetSlamState.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_SetSlamState_Request(type):
    """Metaclass of message 'SetSlamState_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('cartographer_ros_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'cartographer_ros_msgs.srv.SetSlamState_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_slam_state__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_slam_state__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_slam_state__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_slam_state__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_slam_state__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SetSlamState_Request(metaclass=Metaclass_SetSlamState_Request):
    """Message class 'SetSlamState_Request'."""

    __slots__ = [
        '_ram_disk_path',
        '_load_frozen_state',
    ]

    _fields_and_field_types = {
        'ram_disk_path': 'string',
        'load_frozen_state': 'boolean',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.ram_disk_path = kwargs.get('ram_disk_path', str())
        self.load_frozen_state = kwargs.get('load_frozen_state', bool())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.ram_disk_path != other.ram_disk_path:
            return False
        if self.load_frozen_state != other.load_frozen_state:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def ram_disk_path(self):
        """Message field 'ram_disk_path'."""
        return self._ram_disk_path

    @ram_disk_path.setter
    def ram_disk_path(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'ram_disk_path' field must be of type 'str'"
        self._ram_disk_path = value

    @builtins.property
    def load_frozen_state(self):
        """Message field 'load_frozen_state'."""
        return self._load_frozen_state

    @load_frozen_state.setter
    def load_frozen_state(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'load_frozen_state' field must be of type 'bool'"
        self._load_frozen_state = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_SetSlamState_Response(type):
    """Metaclass of message 'SetSlamState_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('cartographer_ros_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'cartographer_ros_msgs.srv.SetSlamState_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_slam_state__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_slam_state__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_slam_state__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_slam_state__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_slam_state__response

            from cartographer_ros_msgs.msg import StatusCode
            if StatusCode.__class__._TYPE_SUPPORT is None:
                StatusCode.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SetSlamState_Response(metaclass=Metaclass_SetSlamState_Response):
    """Message class 'SetSlamState_Response'."""

    __slots__ = [
        '_status',
    ]

    _fields_and_field_types = {
        'status': 'cartographer_ros_msgs/StatusCode',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['cartographer_ros_msgs', 'msg'], 'StatusCode'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from cartographer_ros_msgs.msg import StatusCode
        self.status = kwargs.get('status', StatusCode())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.status != other.status:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def status(self):
        """Message field 'status'."""
        return self._status

    @status.setter
    def status(self, value):
        if __debug__:
            from cartographer_ros_msgs.msg import StatusCode
            assert \
                isinstance(value, StatusCode), \
                "The 'status' field must be a sub message of type 'StatusCode'"
        self._status = value


class Metaclass_SetSlamState(type):
    """Metaclass of service 'SetSlamState'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('cartographer_ros_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'cartographer_ros_msgs.srv.SetSlamState')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__set_slam_state

            from cartographer_ros_msgs.srv import _set_slam_state
            if _set_slam_state.Metaclass_SetSlamState_Request._TYPE_SUPPORT is None:
                _set_slam_state.Metaclass_SetSlamState_Request.__import_type_support__()
            if _set_slam_state.Metaclass_SetSlamState_Response._TYPE_SUPPORT is None:
                _set_slam_state.Metaclass_SetSlamState_Response.__import_type_support__()


class SetSlamState(metaclass=Metaclass_SetSlamState):
    from cartographer_ros_msgs.srv._set_slam_state import SetSlamState_Request as Request
    from cartographer_ros_msgs.srv._set_slam_state import SetSlamState_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
