// DFF -- An Open Source Digital Forensics Framework
// Copyright (C) 2009-2015 ArxSys
// This program is free software, distributed under the terms of
// the GNU General Public License Version 2. See the LICENSE file
// at the top of the source tree.
 
// See http://www.digital-forensic.org for more information about this
// project. Please do not directly contact any of the maintainers of
// DFF for assistance; the project provides a web site, mailing lists
// and IRC channels for your use.

// Author(s):
//  Frederic Baguelin <fba@digital-forensic.org>


#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "crashdialog.hpp"
#include "crashreporter.hpp"
#include "crashdumpinfo.hpp"

#include <QApplication>

void		sendDump(char* version, char* path);

int		main(int argc, char* argv[])
{
  int		c;
  char*		path;
  char*		version;
 
  path = NULL;
  version = NULL;
  while ((c = getopt(argc, argv, "v:p:")) != -1)
    {
      switch (c)
	{
	case 'v':
	  version = optarg;
	  break;
	case 'p':
	  path = optarg;
	  break;
	default:
	  std::cout << "Wrong argument provided to CrashReporter" << std::endl;
	  exit(1);
	}
    }
  if (version == NULL || path == NULL)
    {
      std::cout << "Missing arguments to CrashReporter" << std::endl;
      exit(1);
    }
  QApplication app(argc, argv, true);
  sendDump(version, path);
}


void			sendDump(char* version, char* path)
{
  int			ret;
  CrashDialog*		cdialog;
  CrashReporter*	reporter;
  CrashDumpInfo*	cinfo;

  cinfo = new CrashDumpInfo();
  try
    {
      cinfo->process(path);
    }
  catch (std::string err)
    {
      std::cout << err << std::endl;
      exit(1);
    }
  cdialog = new CrashDialog();
  cdialog->setDetails(cinfo->details());
  ret = cdialog->exec();
  if (cdialog->reportEnabled())
    {
      reporter = createCrashReporter();
      reporter->setMinidumpPath(path);
      reporter->setVersion(version);
      reporter->setComment(cdialog->userComment());
      if (cdialog->proxyEnabled())
	{
	  reporter->setProxyHost(cdialog->proxyHost());
	  reporter->setProxyUser(cdialog->proxyUser());
	  reporter->setProxyPassword(cdialog->proxyPassword());
	}
      reporter->sendReport();
    }
  if (ret == CrashDialog::Exit)
    exit(1);
}

