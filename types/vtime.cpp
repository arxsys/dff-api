/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2013 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http://www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal J. <sja@digital-forensic.org>
 */

#include "vtime.hpp"

vtime::vtime() : year(0), month(0), day(0), hour(0), minute(0), second(0), usecond(0), wday(0), yday(0), dst(0)
{
}

vtime::vtime(int y, int mo, int d, int h, int mi, int s, int us) : year(y), month(mo), day(d), hour(h), minute(mi), second(s), usecond(us), wday(0), yday(0), dst(0)
{
}

vtime::vtime(uint16_t dos_time, uint16_t dos_date) : hour(0), minute(0), second(0), usecond(0), wday(0), yday(0), dst(0)
{
  this->day = (dos_date & 31);
  this->month = ((dos_date >> 5) & 15);
  this->year = ((dos_date >> 9) + 1980);

  if (dos_time != 0)
  {
    this->second = (dos_time & 31) * 2;
    this->minute = ((dos_time >> 5) & 63);
    this->hour = (dos_time >> 11);
  }
}

vtime::vtime(uint64_t value, int type = 0) : year(0), month(0), day(0), hour(0), minute(0), second(0), dst(0), wday(0), yday(0), usecond(0)
{
  if (value > 0)
  {
    if (type == TIME_MS_64)
    {
      value -= NANOSECS_1601_TO_1970;
      value /= 10000000;
    }
    struct tm   date;
    #ifdef WIN32
    if (_gmtime64_s(&date, (__time64_t*)&value) == 0)
    #else
    if (gmtime_r((time_t *)&value, &date) != NULL)
    #endif
    {
      this->year = date.tm_year + 1900;
      this->month = date.tm_mon + 1;
      this->day = date.tm_mday;
      this->hour = date.tm_hour;
      this->minute = date.tm_min;
      this->second = date.tm_sec;
      this->dst = date.tm_isdst;
      this->wday = date.tm_wday;
      this->yday = date.tm_yday;
    }
  }
}

vtime::vtime(std::string ts) : year(0), month(0), day(0), hour(0), minute(0), second(0), dst(0), wday(0), yday(0), usecond(0)
{
  size_t			midx;
  std::string			date;
  std::string			time;

  if ((midx = ts.find("T")) != std::string::npos)
  {
    date = ts.substr(0, midx);
    this->__setFromDate(date);
    time = ts.substr(midx+1);
    this->__setFromTime(time);
  }
  else if (ts.find(":") != std::string::npos && ts.find("-") == std::string::npos)
    this->__setFromTime(ts);
  else if (ts.find("-") != std::string::npos && ts.find(":") == std::string::npos)
    this->__setFromDate(ts);
}

void	vtime::__setFromDate(std::string date)
{
  size_t	cpos;
  size_t	opos;

  cpos = date.find("-");
  if (cpos != std::string::npos)
    {
      std::istringstream(date.substr(0, cpos)) >> this->year;
      opos = cpos + 1;
      cpos = date.find("-", opos);
      if (cpos != std::string::npos)
	{
	  std::istringstream(date.substr(opos, cpos)) >> this->month;
	  if (cpos < date.size())
	    std::istringstream(date.substr(cpos+1)) >> this->day;
	}
      else if (opos < std::string::npos)
	std::istringstream(date.substr(opos)) >> this->month;
    }
  else
    std::istringstream(date.substr(0)) >> this->year;
}

void	vtime::__setFromTime(std::string time)
{
  size_t	cpos;
  size_t	opos;

  cpos = time.find(":");
  if (cpos != std::string::npos)
    {
      std::istringstream(time.substr(0, cpos)) >> this->hour;
      opos = cpos + 1;
      cpos = time.find(":", opos);
      if (cpos != std::string::npos)
	{
	  std::istringstream(time.substr(opos, cpos)) >> this->minute;
	  if (cpos < time.size())
	    std::istringstream(time.substr(cpos+1)) >> this->second;
	}
      else if (opos < std::string::npos)
	std::istringstream(time.substr(opos)) >> this->minute;
    }
  else
    std::istringstream(time.substr(0)) >> this->hour;
}

bool	vtime::operator==(vtime* v)
{
  if (v != NULL)
    return ((this->year == v->year) && 
	    (this->month == v->month) &&
	    (this->day == v->day) &&
	    (this->hour == v->hour) &&
	    (this->minute == v->minute) &&
	    (this->second == v->second) &&
	    (this->usecond == v->usecond));
  else
    return false;
}

bool	vtime::operator!=(vtime* v)
{
  return !(this->operator==(v));
}

bool	vtime::operator>(vtime* v)
{
  if (v != NULL)
    return ((this->year > v->year) || ((this->year == v->year) && ((this->month > v->month) || ((this->month == v->month) && ((this->day > v->day) || ((this->day == v->day) && ((this->hour > v->hour) || ((this->hour == v->hour) && ((this->minute > v->minute) || ((this->minute == v->minute) && ((this->second > v->second) || ((this->second == v->second) && (this->usecond > v->usecond)))))))))))));
  else
    return true;
}

bool	vtime::operator<(vtime* v)
{
  return !(this->operator>(v));
}

bool	vtime::operator>=(vtime* v)
{
  if (v != NULL)
    return ((this->year > v->year) || ((this->year == v->year) && ((this->month > v->month) || ((this->month == v->month) && ((this->day > v->day) || ((this->day == v->day) && ((this->hour > v->hour) || ((this->hour == v->hour) && ((this->minute > v->minute) || ((this->minute == v->minute) && ((this->second > v->second) || ((this->second == v->second) && ((this->usecond > v->usecond) || (this->usecond == v->usecond))))))))))))));
  else
    return true;
}

bool	vtime::operator<=(vtime* v)
{
  return !(this->operator>=(v));
}

vtime::~vtime()
{
}

vtimeMS128::vtimeMS128(char *_time) : vtime()
{
  if (_time == NULL)
    throw std::string("vtimeMS128, time is NULL");
  
  uint16_t* t = (uint16_t*)_time;
  this->year = *t++;
  this->month = *t++;
  this->wday = *t++;
  this->day = *t++;

  this->hour = *t++;
  this->minute = *t++;
  this->second = *t++;
  this->usecond = *t++;
}
