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

#include <iostream>
#include <string>

#define ReportExecutable "CrashReporter"

#ifndef WIN32

#include <unistd.h>

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

#include <windows.h>
#include <stdio.h>
#include <tchar.h>	
	
bool	dumpCallback(const wchar_t* _dump_dir, const wchar_t* _minidump_id, void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool success)
{
  STARTUPINFO		si;
  PROCESS_INFORMATION	pi;
  
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  std::wstring args(L"CrashReporer -p ");
  args += std::wstring(_dump_dir);
  args += std::wstring(_minidump_id);
  args += std::wstring(L".dmp -v ");
  args += std::wstring((char*)context, (char*)context+strlen((char*)context));
  if (!CreateProcessW(L"dff\\api\\exceptions\\reporter\\CrashReporter.exe", 
		      (LPWSTR)args.c_str(),
		      NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOW)&si, &pi))
    {
      printf("Create Process failed (%d)\n", GetLastError());
      TerminateProcess(GetCurrentProcess(), 1);
      return false;
    }
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
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
	TCHAR tempPath[1024];
	std::wstring wpath;
	
	GetTempPath(1024, tempPath);
	wpath = std::wstring(tempPath);
	this->__eh = new google_breakpad::ExceptionHandler(wpath,
							   NULL,//DmpFilter, 
							   dumpCallback,//DmpCallback 
  							   (void*)this->__version.c_str(), 
							   true);
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
