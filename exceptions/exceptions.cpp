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

#include "exceptions.hpp"

#include <unistd.h>
#include <iostream>
#include <string>

#define ReportExecutable "CrashReporter"

#ifndef WIN32
static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                         void* context,
                         bool succeeded)
{
  pid_t	pid;

  std::cout << "Dumping to " << descriptor.path() << std::endl;

  pid = fork();
  if (pid == -1)
    {
      printf("Cannot fork. Please send the following crash report to contact@arxsys.fr:\n%s\n", descriptor.path());
      return false;
    }
  else if (pid == 0)
    {
      execl("dff/api/exceptions/reporter/CrashReporter", "CrashReporter", "-p", descriptor.path(), "-v", (char*)context, NULL);
      exit(1);
    }
  return succeeded;
}

#else

bool dumpCallback(const wchar_t* _dump_dir, const wchar_t* _minidump_id, void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool success)
  std::wcout << "Dumping to " << _dump_dir << std::endl;
  return true;
}
#endif


CrashHandler::CrashHandler() : __version(""), __eh(NULL)
{
}

CrashHandler::~CrashHandler()
{

}


void	CrashHandler::setVersion(std::string version)
{
  this->__version = version;
}


void	CrashHandler::setHandler()
{
  #ifndef WIN32
	google_breakpad::MinidumpDescriptor descriptor("/tmp");
	this->__eh = new google_breakpad::ExceptionHandler(descriptor, 
							   NULL,//DmpFilter, 
							   dumpCallback, 
							   (void*)this->__version.c_str(), 
							   true,
							   -1);
  #else
	this->__eh = new google_breakpad::ExceptionHandler(L"C:\\Users\\builder\\Desktop", 
							   NULL,//DmpFilter, 
							   dumpCallback,//DmpCallback 
							   0,
							   true);
							   // google_breakpad::ExceptionHandler::HANDLER_ALL,
							   // MiniDumpNormal,
							   // L"",
							   // 0);	
  #endif
}


void	CrashHandler::unsetHandler()
{
}


envError::envError(std::string emsg)
{
  error = emsg;
}

vfsError::vfsError(std::string emsg)
{
    error = emsg;
}
