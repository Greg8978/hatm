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
 * Auto-generated by genmsg_cpp from file /home/gmilliez/openrobots/src/acapela-genom/ros/client/ros/acapela/msg/genom_state_activity.msg
 *
 */


#ifndef ACAPELA_MESSAGE_GENOM_STATE_ACTIVITY_H
#define ACAPELA_MESSAGE_GENOM_STATE_ACTIVITY_H


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
struct genom_state_activity_
{
  typedef genom_state_activity_<ContainerAllocator> Type;

  genom_state_activity_()
    : id(0)
    , name()
    , state()  {
    }
  genom_state_activity_(const ContainerAllocator& _alloc)
    : id(0)
    , name(_alloc)
    , state(_alloc)  {
    }



   typedef uint16_t _id_type;
  _id_type id;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _name_type;
  _name_type name;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _state_type;
  _state_type state;




  typedef boost::shared_ptr< ::acapela::genom_state_activity_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::acapela::genom_state_activity_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct genom_state_activity_

typedef ::acapela::genom_state_activity_<std::allocator<void> > genom_state_activity;

typedef boost::shared_ptr< ::acapela::genom_state_activity > genom_state_activityPtr;
typedef boost::shared_ptr< ::acapela::genom_state_activity const> genom_state_activityConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::acapela::genom_state_activity_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::acapela::genom_state_activity_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::acapela::genom_state_activity_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::acapela::genom_state_activity_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::genom_state_activity_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::acapela::genom_state_activity_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::genom_state_activity_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::acapela::genom_state_activity_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::acapela::genom_state_activity_<ContainerAllocator> >
{
  static const char* value()
  {
    return "0c2fd95b7c6bed2bf405239826f0f7a4";
  }

  static const char* value(const ::acapela::genom_state_activity_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x0c2fd95b7c6bed2bULL;
  static const uint64_t static_value2 = 0xf405239826f0f7a4ULL;
};

template<class ContainerAllocator>
struct DataType< ::acapela::genom_state_activity_<ContainerAllocator> >
{
  static const char* value()
  {
    return "acapela/genom_state_activity";
  }

  static const char* value(const ::acapela::genom_state_activity_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::acapela::genom_state_activity_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# IDL struct ::genom::state::activity\n\
uint16 id\n\
string name\n\
string state\n\
\n\
";
  }

  static const char* value(const ::acapela::genom_state_activity_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::acapela::genom_state_activity_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.id);
      stream.next(m.name);
      stream.next(m.state);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct genom_state_activity_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::acapela::genom_state_activity_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::acapela::genom_state_activity_<ContainerAllocator>& v)
  {
    s << indent << "id: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.id);
    s << indent << "name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.name);
    s << indent << "state: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.state);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ACAPELA_MESSAGE_GENOM_STATE_ACTIVITY_H
