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

if (LIBTRE_FOUND)
  if(WIN32)
    file(COPY ${LIBTRE_RUNTIME_LIBRARY} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  endif(WIN32)
endif (LIBTRE_FOUND)

dff_cpp_api(search
  CPP_FILES search.cpp boyer_moore.cpp
  SWIG_FILE libsearch.i
  LINK_LIBRARIES ${LIBTRE_LIBRARY}
  EXTRA_FILES ${LIBTRE_RUNTIME_LIBRARY} __init__.py
  )
