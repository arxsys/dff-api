# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Solal Jacob <sja@digital-forensic.org>

include_directories("../destruct")

#dff_cpp_api(session
  #SWIG_FILE libsession.i
  #CPP_FILES session.cpp 
  #LINK_LIBRARIES destruct _destruct vfs)
#XXX add case.py

#set(session_libfiles
  #session.cpp
  #configuration.cpp
#)

#add_library(session SHARED ${session_libfiles})
