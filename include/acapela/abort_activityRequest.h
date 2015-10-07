/* Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Auto-generated by genmsg_cpp from file /home/gmilliez/openrobots/src/acapela-genom/ros/client/ros/acapela/srv/abort_activity.srv
 *
 */


#ifndef ACAPELA_MESSAGE_ABORT_ACTIVITYREQUEST_H
#define ACAPELA_MESSAGE_ABORT_ACTIVITYREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace acapela
{
template <class ContainerAllocator>
struct abort_activityRequest_
{
  typedef abort_activityRequest_<ContainerAllocator> Type;

  abort_activityRequest_()
    : activity(0)  {
    }
  abort_activityRequest_(const ContainerAllocator& _alloc)
    : activity(0)  {
    }



   typedef uint32_t _activity_type;
  _activity_type activity;




  typedef boost::shared_ptr< ::acapela::abort_activityRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::acapela::abort_activityRequest_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct abort_activityRequest_

typedef ::acapela::abort_activityRequest_<std::allocator<void> > abort_activityRequest;

typedef boost::shared_ptr< ::acapela::abort_activityRequest > abort_activityRequestPtr;
typedef boost::shared_ptr< ::acapela::abort_activityRequest const> abort_activityRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::acapela::abort_activityRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::acapela::abort_activityRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace acapela

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'acapela': ['acapela/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::acapela::abort_activityRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::acapela::abort_activityRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::abort_activityRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::abort_activityRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::abort_activityRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::abort_activityRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::acapela::abort_activityRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bbd20bba9542755cfbf3efd56184eaeb";
  }

  static const char* value(const ::acapela::abort_activityRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xbbd20bba9542755cULL;
  static const uint64_t static_value2 = 0xfbf3efd56184eaebULL;
};

template<class ContainerAllocator>
struct DataType< ::acapela::abort_activityRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "acapela/abort_activityRequest";
  }

  static const char* value(const ::acapela::abort_activityRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::acapela::abort_activityRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "\n\
uint32 activity\n\
\n\
";
  }

  static const char* value(const ::acapela::abort_activityRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::acapela::abort_activityRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.activity);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct abort_activityRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::acapela::abort_activityRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::acapela::abort_activityRequest_<ContainerAllocator>& v)
  {
    s << indent << "activity: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.activity);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ACAPELA_MESSAGE_ABORT_ACTIVITYREQUEST_H