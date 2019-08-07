// Generated by gencpp from file dector/ColorBool.msg
// DO NOT EDIT!


#ifndef DECTOR_MESSAGE_COLORBOOL_H
#define DECTOR_MESSAGE_COLORBOOL_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace dector
{
template <class ContainerAllocator>
struct ColorBool_
{
  typedef ColorBool_<ContainerAllocator> Type;

  ColorBool_()
    : color()
    , data(false)  {
    }
  ColorBool_(const ContainerAllocator& _alloc)
    : color(_alloc)
    , data(false)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _color_type;
  _color_type color;

   typedef uint8_t _data_type;
  _data_type data;





  typedef boost::shared_ptr< ::dector::ColorBool_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dector::ColorBool_<ContainerAllocator> const> ConstPtr;

}; // struct ColorBool_

typedef ::dector::ColorBool_<std::allocator<void> > ColorBool;

typedef boost::shared_ptr< ::dector::ColorBool > ColorBoolPtr;
typedef boost::shared_ptr< ::dector::ColorBool const> ColorBoolConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::dector::ColorBool_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::dector::ColorBool_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace dector

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'sensor_msgs': ['/opt/ros/kinetic/share/sensor_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'dector': ['/home/vwpp/wangshuai_ws/src/dector/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::dector::ColorBool_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::dector::ColorBool_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dector::ColorBool_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dector::ColorBool_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dector::ColorBool_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dector::ColorBool_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::dector::ColorBool_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bc7e85d2f020744b4e52b3f6c9290180";
  }

  static const char* value(const ::dector::ColorBool_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xbc7e85d2f020744bULL;
  static const uint64_t static_value2 = 0x4e52b3f6c9290180ULL;
};

template<class ContainerAllocator>
struct DataType< ::dector::ColorBool_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dector/ColorBool";
  }

  static const char* value(const ::dector::ColorBool_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::dector::ColorBool_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string color\n\
bool data\n\
";
  }

  static const char* value(const ::dector::ColorBool_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::dector::ColorBool_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.color);
      stream.next(m.data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ColorBool_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dector::ColorBool_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::dector::ColorBool_<ContainerAllocator>& v)
  {
    s << indent << "color: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.color);
    s << indent << "data: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.data);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DECTOR_MESSAGE_COLORBOOL_H