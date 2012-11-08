/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
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
#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#ifndef WIN32
	#include <stdint.h>
#elif _MSC_VER >= 1600
	#include <stdint.h>
#else
	#include "wstdint.h"
#endif
#include <iostream>
#include <vector>

class Device
{
public:
				Device();
  virtual			~Device();
  virtual wchar_t*	 	blockDevice(void);
  virtual wchar_t*		serialNumber(void);
  virtual wchar_t*	 	model(void);
  virtual uint64_t		size(void);
};

class DeviceList
{
public:
			        DeviceList();
};

#endif
