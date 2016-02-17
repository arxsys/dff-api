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

#ifndef __VTIME_HPP__
#define __VTIME_HPP__

#ifndef WIN32   //EVERYWHERE CHANGER CA PLEASE C degueux lasse
#include <stdint.h>
#elif _MSC_VER >= 1600
#include <stdint.h>
#else
#include "wstdint.h"
#endif

#include <time.h>
#include <string>

#include "export.hpp"
//XXX grep TIME_FIX ou FIX_TIME ?

//XXX XXX XXX usename space
//Virer le DTime degeux de jey ... on a une class pour ca qu elle connard

namespace DFF
{

/**
 *  DateTime
 */
//class TimeDelta
//{}

#ifdef WIN32
#define gmtimex(timet, structtm)\
   _gmtime_64(timet)
#else
#define gmtimex(timet, structtm)\
  gmtime_r(timet, structtm)
#endif

/**
 *  This class store time in UNIX epoch time GMT
 *  derivated class must pass epoch time as time GMT (converted from time zone)
 *  then user can specify a global time zone that will affect returned time informations.
 */
class vtime //Destruct to be serializable ? RENAME DateTime and use namespace !
{
private:
  static const int     __cumdays[12]; //XXX ambigous name :)
  static int64_t       __globalTimeZoneOffset;
  int64_t              __epochTime;
protected:
  int64_t              __timegm(struct tm*); 
public:
  EXPORT explicit       vtime(int64_t epochTime);
  EXPORT                vtime(vtime const&);
  EXPORT		vtime(const std::string&);
  EXPORT 		vtime(int32_t year, int32_t month, int32_t day, int32_t minute, int32_t hour, int32_t second);
  EXPORT virtual	~vtime();

  EXPORT bool           operator==(const vtime& other) const; //Take care to compare to None with is None in python not == None which can't convert None to vtime
  EXPORT bool           operator!=(const vtime& other) const;
  EXPORT bool		operator<(const vtime& other) const;
  EXPORT bool           operator>(const vtime& other) const;
  EXPORT bool		operator<=(const vtime& other) const;
  EXPORT bool           operator>=(const vtime& other) const;
/*
  need time span if want to implement
  EXPORT TimeDelta operator+(const vtime& other) const;
  EXPORT TimeDelta operator-(const vtime& other) const;
  const vtime& operator+=(TimeDelta delta);
  const vtime& operator-=(TimeDelta delta);
*/

  EXPORT int64_t                epochTime(void) const;
  EXPORT void                   epochTime(int64_t);
  EXPORT int32_t                globalTimeZone(void) const;
  EXPORT void                   globalTimeZone(int32_t timeZone);

  EXPORT const std::string      toString(void) const;
  EXPORT const std::string      toISOString(void) const;

  EXPORT int32_t                year(void) const;
  EXPORT int32_t                month(void) const;
  EXPORT int32_t                day(void) const;
  EXPORT int32_t                hour(void) const;
  EXPORT int32_t                minute(void) const;
  EXPORT int32_t                second(void) const;
  EXPORT int32_t                dayOfWeek(void) const;
  EXPORT int32_t                dayOfYear(void) const;
  //EXPORT uint8_t              dst(void) const;
};


/**
 *  DosDateTime
 */
class DosDateTime : public vtime 
{
#define SECONDS_PER_MIN	  60
#define SECONDS_PER_HOUR  (60 * 60)
#define SECONDS_PER_DAY	  (SECONDS_PER_HOUR * 24)
#define DAYS_DELTA	  (365 * 10 + 2)
#define YEAR_2100	  120
#define IS_LEAP_YEAR(y)	(!((y) & 3) && (y) != YEAR_2100)
public:
  EXPORT DosDateTime(uint16_t time, uint16_t date);
private:
  static const time_t days_in_year[];
};

/**
 *  MS64DateTime
 */
class MS64DateTime : public vtime //MSFILETIME  // WindowsFileTime // MicrosoftFileTime
{
#if __WORDSIZE == 64
  #define SECONDS_FROM_1601_TO_1970  (uint64_t)(116444736000000000UL)
#else
  #define SECONDS_FROM_1601_TO_1970  (uint64_t)(116444736000000000ULL)
#endif
public:
  EXPORT MS64DateTime(uint64_t);
};

/**
 *  MS128DateTime
 */
class MS128DateTime : public vtime
{
public:
  EXPORT  MS128DateTime(char *);
}; 

/**
 *  HFSDateTime
 */
class HFSDateTime : public vtime
{
#define SECONDS_FROM_1904_TO_1970  (uint64_t)(2082844800ULL)
public:
  EXPORT HFSDateTime(uint32_t);
};

}

#endif
