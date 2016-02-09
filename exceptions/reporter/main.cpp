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

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "crashdialog.hpp"
#include "crashreporter.hpp"
#include "crashdumpinfo.hpp"

#include <QApplication>
#include <QDesktopWidget>
#include <QObject>

void		sendDump(int argc, char* argv[], char* version, char* path);
void		usage();
void		parseArguments(int argc, char* argv[], char** version, char** path);

int		main(int argc, char* argv[])
{
  char*		path;
  char*		version;
 
  path = NULL;
  version = NULL;
  parseArguments(argc, argv, &version, &path);
  sendDump(argc, argv, version, path);
}


void	parseArguments(int argc, char* argv[], char** version, char** path)
{
  int	i;

  if (argc != 5)
    usage();
  for (i = 0; i != 5; i++)
    {
      if ((strncmp(argv[i], "-p", 2) == 0) && i < 5 && (strncmp(argv[i+1], "-v", 2) != 0))
	*path = argv[i+1];
      if ((strncmp(argv[i], "-v", 2) == 0) && i < 5 && (strncmp(argv[i+1], "-p", 2) != 0))
	*version = argv[i+1];
    }
  if (version == NULL || path == NULL)
    usage();
}


void	usage()
{
  std::cout << "usage: CrashReporter -v version -p path" << std::endl;
  exit(1);
}

void			sendDump(int argc, char* argv[], char* version, char* path)
{
  int			ret;
  std::string		msg;
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
      delete cinfo;
      exit(1);
    }
  QApplication app(argc, argv, true);
  cdialog = new CrashDialog();
  cdialog->setDetails(cinfo->details());
  cdialog->setVersion(version);
  cdialog->setMinidumpPath(path);
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
      if (!reporter->sendReport())
	{ 
	  msg = "Error while uploading dump " + reporter->minidumpPath();
	  msg += "\nYou can send it by mail at the following address : contact@arxsys.fr";
	  QMessageBox::critical(QDesktopWidget().screen(), "Crash Reporter", QObject::tr(msg.c_str()));
	}
      else
	{
	  msg = "Dump " + reporter->minidumpPath() + " successfully uploaded";
	  msg += "\nand is available at " + reporter->viewUrl();
	  msg += "\n\nThanks for your support!";
	  QMessageBox::information(QDesktopWidget().screen(), "Crash Reporter", QObject::tr(msg.c_str()));
	}
    }
  if (ret == CrashDialog::Exit)
  {
    delete cinfo;
    exit(1);
  }
}

