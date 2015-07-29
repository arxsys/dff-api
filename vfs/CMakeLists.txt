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
#  Frederic Baguelin <fba@digital-forensic.org>
#  Solal Jacob <sja@digital-forensic.org>


if (WIN32)
  set(libbreakpad "${CMAKE_BINARY_DIR}/dff/api/exceptions/breakpad/breakpad_client.lib")
elseif (UNIX)
  set(libbreakpad "${CMAKE_BINARY_DIR}/dff/api/exceptions/breakpad/libbreakpad_client.a")
endif(WIN32)


dff_cpp_api(vfs 
  SWIG_FILE libvfs.i
  CPP_FILES rc.cpp tags.cpp node.cpp vlink.cpp vfile.cpp vfs.cpp fso.cpp mfso.cpp filemapping.cpp fdmanager.cpp cache.cpp iostat.cpp rootnode.cpp
  LINK_LIBRARIES datatype search exceptions types events ${libbreakpad}
  EXTRA_FILES vfs.py __init__.py iodevice.py extract.py
  INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad"
  )
