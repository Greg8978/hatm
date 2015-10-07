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
 * Auto-generated by gensrv_cpp from file /home/gmilliez/openrobots/src/acapela-genom/ros/client/ros/acapela/srv/abort_activity.srv
 *
 */


#ifndef ACAPELA_MESSAGE_ABORT_ACTIVITY_H
#define ACAPELA_MESSAGE_ABORT_ACTIVITY_H

#include <ros/service_traits.h>


#include <acapela/abort_activityRequest.h>
#include <acapela/abort_activityResponse.h>


namespace acapela
{

struct abort_activity
{

typedef abort_activityRequest Request;
typedef abort_activityResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct abort_activity
} // namespace acapela


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::acapela::abort_activity > {
  static const char* value()
  {
    return "6f7863a098062dd05d0c2aa1e1caedf6";
  }

  static const char* value(const ::acapela::abort_activity&) { return value(); }
};

template<>
struct DataType< ::acapela::abort_activity > {
  static const char* value()
  {
    return "acapela/abort_activity";
  }

  static const char* value(const ::acapela::abort_activity&) { return value(); }
};


// service_traits::MD5Sum< ::acapela::abort_activityRequest> should match 
// service_traits::MD5Sum< ::acapela::abort_activity > 
template<>
struct MD5Sum< ::acapela::abort_activityRequest>
{
  static const char* value()
  {
    return MD5Sum< ::acapela::abort_activity >::value();
  }
  static const char* value(const ::acapela::abort_activityRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::acapela::abort_activityRequest> should match 
// service_traits::DataType< ::acapela::abort_activity > 
template<>
struct DataType< ::acapela::abort_activityRequest>
{
  static const char* value()
  {
    return DataType< ::acapela::abort_activity >::value();
  }
  static const char* value(const ::acapela::abort_activityRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::acapela::abort_activityResponse> should match 
// service_traits::MD5Sum< ::acapela::abort_activity > 
template<>
struct MD5Sum< ::acapela::abort_activityResponse>
{
  static const char* value()
  {
    return MD5Sum< ::acapela::abort_activity >::value();
  }
  static const char* value(const ::acapela::abort_activityResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::acapela::abort_activityResponse> should match 
// service_traits::DataType< ::acapela::abort_activity > 
template<>
struct DataType< ::acapela::abort_activityResponse>
{
  static const char* value()
  {
    return DataType< ::acapela::abort_activity >::value();
  }
  static const char* value(const ::acapela::abort_activityResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ACAPELA_MESSAGE_ABORT_ACTIVITY_H