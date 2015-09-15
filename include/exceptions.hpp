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


#ifndef __EXCEPTIONS_HH__
#define __EXCEPTIONS_HH__

#include <string>
#include "export.hpp"

#ifndef WIN32
  #include "client/linux/handler/exception_handler.h"
#else
  #include "client/windows/handler/exception_handler.h"
#endif


class CrashHandler
{
private:
  std::string				__version;
  google_breakpad::ExceptionHandler	*__eh;
public:
  CrashHandler();
  ~CrashHandler();
  void	setVersion(std::string);
  void	setHandler();
  void	unsetHandler();
};

class envError
{
public:
  std::string error;	
  EXPORT envError(std::string msg);
};

class vfsError
{
public:
  std::string error;	

 EXPORT vfsError(std::string msg);
};


#endif 
