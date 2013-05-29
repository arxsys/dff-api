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
#  Christophe Malinge <cma@digital-forensic.org>
#  Frederic Baguelin <fba@digital-forensic.org>

if(WIN32 AND NOT UNIX)
  if (MAGIC_LIBRARIES_PATH)
    find_library(MAGIC_LIBRARIES NAMES libgnurx-0 libmagic-1 PATHS MAGIC_LIBRARY_PATH)
    if (MAGIC_LIBRARIES-NOTFOUND)
      message (FATAL_ERROR "Cannot find magic libraries. Please provide the right path to magic libraries")
    endif()
  else ()
    message (FATAL_ERROR "MAGIC_LIBRARIES_PATH not set. Cannot find needed libraries. Please set the path to magic libraries")
  endif ()
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
  install_file(
    __init__.py
    magic.mgc
    magic.py
    ${MAGIC_LIBRARIES}
    )
endif(WIN32 AND NOT UNIX)

