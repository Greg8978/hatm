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
 * Auto-generated by genmsg_cpp from file /home/gmilliez/openrobots/src/acapela-genom/ros/client/ros/acapela/msg/InitGoal.msg
 *
 */


#ifndef ACAPELA_MESSAGE_INITGOAL_H
#define ACAPELA_MESSAGE_INITGOAL_H


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
struct InitGoal_
{
  typedef InitGoal_<ContainerAllocator> Type;

  InitGoal_()
    : server()
    , verbose(0)  {
    }
  InitGoal_(const ContainerAllocator& _alloc)
    : server(_alloc)
    , verbose(0)  {
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _server_type;
  _server_type server;

   typedef int32_t _verbose_type;
  _verbose_type verbose;




  typedef boost::shared_ptr< ::acapela::InitGoal_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::acapela::InitGoal_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct InitGoal_

typedef ::acapela::InitGoal_<std::allocator<void> > InitGoal;

typedef boost::shared_ptr< ::acapela::InitGoal > InitGoalPtr;
typedef boost::shared_ptr< ::acapela::InitGoal const> InitGoalConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::acapela::InitGoal_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::acapela::InitGoal_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace acapela

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'actionlib_msgs': ['/opt/ros/groovy/share/actionlib_msgs/msg'], 'std_msgs': ['/opt/ros/groovy/share/std_msgs/msg'], 'acapela': ['acapela/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::acapela::InitGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::acapela::InitGoal_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::InitGoal_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::InitGoal_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::InitGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::InitGoal_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::acapela::InitGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "008fc9858508e449fd998902e4cbc9e7";
  }

  static const char* value(const ::acapela::InitGoal_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x008fc9858508e449ULL;
  static const uint64_t static_value2 = 0xfd998902e4cbc9e7ULL;
};

template<class ContainerAllocator>
struct DataType< ::acapela::InitGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "acapela/InitGoal";
  }

  static const char* value(const ::acapela::InitGoal_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::acapela::InitGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
# service Init\n\
string server\n\
int32 verbose\n\
\n\
";
  }

  static const char* value(const ::acapela::InitGoal_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::acapela::InitGoal_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.server);
      stream.next(m.verbose);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct InitGoal_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::acapela::InitGoal_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::acapela::InitGoal_<ContainerAllocator>& v)
  {
    s << indent << "server: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.server);
    s << indent << "verbose: ";
    Printer<int32_t>::stream(s, indent + "  ", v.verbose);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ACAPELA_MESSAGE_INITGOAL_H
