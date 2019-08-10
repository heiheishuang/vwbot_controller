// Generated by gencpp from file dector/PointServerResponse.msg
// DO NOT EDIT!


#ifndef DECTOR_MESSAGE_POINTSERVERRESPONSE_H
#define DECTOR_MESSAGE_POINTSERVERRESPONSE_H


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
struct PointServerResponse_
{
  typedef PointServerResponse_<ContainerAllocator> Type;

  PointServerResponse_()
    : data(false)  {
    }
  PointServerResponse_(const ContainerAllocator& _alloc)
    : data(false)  {
  (void)_alloc;
    }



   typedef uint8_t _data_type;
  _data_type data;





  typedef boost::shared_ptr< ::dector::PointServerResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dector::PointServerResponse_<ContainerAllocator> const> ConstPtr;

}; // struct PointServerResponse_

typedef ::dector::PointServerResponse_<std::allocator<void> > PointServerResponse;

typedef boost::shared_ptr< ::dector::PointServerResponse > PointServerResponsePtr;
typedef boost::shared_ptr< ::dector::PointServerResponse const> PointServerResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::dector::PointServerResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::dector::PointServerResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace dector

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'sensor_msgs': ['/opt/ros/kinetic/share/sensor_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'dector': ['/home/wang_shuai/def/src/dector/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::dector::PointServerResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::dector::PointServerResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dector::PointServerResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dector::PointServerResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dector::PointServerResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dector::PointServerResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::dector::PointServerResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "8b94c1b53db61fb6aed406028ad6332a";
  }

  static const char* value(const ::dector::PointServerResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x8b94c1b53db61fb6ULL;
  static const uint64_t static_value2 = 0xaed406028ad6332aULL;
};

template<class ContainerAllocator>
struct DataType< ::dector::PointServerResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dector/PointServerResponse";
  }

  static const char* value(const ::dector::PointServerResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::dector::PointServerResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bool data\n\
";
  }

  static const char* value(const ::dector::PointServerResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::dector::PointServerResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct PointServerResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dector::PointServerResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::dector::PointServerResponse_<ContainerAllocator>& v)
  {
    s << indent << "data: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.data);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DECTOR_MESSAGE_POINTSERVERRESPONSE_H