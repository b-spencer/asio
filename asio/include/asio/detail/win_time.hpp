//
// win_time.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003, 2004 Christopher M. Kohlhoff (chris@kohlhoff.com)
//
// Permission to use, copy, modify, distribute and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appears in all copies and that both the copyright
// notice and this permission notice appear in supporting documentation. This
// software is provided "as is" without express or implied warranty, and with
// no claim as to its suitability for any purpose.
//

#ifndef ASIO_DETAIL_WIN_TIME_HPP
#define ASIO_DETAIL_WIN_TIME_HPP

#include "asio/detail/push_options.hpp"

#if defined(_WIN32)

#include "asio/detail/socket_types.hpp"

namespace asio {
namespace detail {

class win_time
{
public:
  // Construct with particular time in seconds and microseconds.
  win_time(long seconds = 0, long microseconds = 0)
    : sec_(seconds),
      usec_(microseconds)
  {
    normalise();
  }

  // Get the seconds component of the time.
  long sec() const
  {
    return sec_;
  }

  // Set the seconds component of the time.
  void sec(long seconds)
  {
    sec_ = seconds;
    normalise();
  }

  // Get the microseconds component of the time.
  long usec() const
  {
    return usec_;
  }

  // Set the microseconds component of the time.
  void usec(long microseconds)
  {
    usec_ = microseconds;
    normalise();
  }

  // Get the current time.
  static win_time now()
  {
    SYSTEMTIME system_time;
    ::GetSystemTime(&system_time);
    FILETIME file_time;
    ::SystemTimeToFileTime(&system_time, &file_time);
    const LONGLONG FILETIME_to_ctime = 116444736000000000;
    LONGLONG file_time_64 = file_time.dwHighDateTime;
    file_time_64 <<= 32;
    file_time_64 += file_time.dwLowDateTime;
    file_time_64 -= FILETIME_to_ctime;
    return win_time(file_time_64 / 10000000, (file_time_64 / 10) % 1000000);
  }

  // Addition operator.
  void operator+=(const win_time& t)
  {
    sec_ += t.sec_;
    usec_ += t.usec_;
    normalise();
  }

  // Subtraction operator.
  void operator-=(const win_time& t)
  {
    sec_ -= t.sec_;
    usec_ -= t.usec_;
    normalise();
  }

  // Equality operator.
  friend bool operator==(const win_time& a, const win_time& b)
  {
    return a.sec_ == b.sec_ && a.usec_ == b.usec_;
  }

  // Comparison operator.
  friend bool operator<(const win_time& a, const win_time& b)
  {
    if (a.sec_ < b.sec_)
      return true;
    if (a.sec_ == b.sec_ && a.usec_ < b.usec_)
      return true;
    return false;
  }

private:
  // Normalise the time, i.e. carry over any excess microseconds into the
  // seconds field.
  void normalise()
  {
    sec_ += usec_ / 1000000;
    usec_ = usec_ % 1000000;
    if (usec_ < 0 && sec_ > 0)
    {
      usec_ += 1000000;
      sec_ -= 1;
    }
  }

  long sec_;
  long usec_;
};

} // namespace detail
} // namespace asio

#endif // defined(_WIN32)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_WIN_TIME_HPP
