# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2011 ArxSys
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
#  Romain Bertholon <rbe@digital-forensic.org>
#  Christophe Malinge <cma@digital-forensic.org>

if(EXISTS ${CLUCENE_CONFIG_INCLUDE_PATH} AND EXISTS ${CLUCENE_LIBRARY})
  find_package(SWIG REQUIRED)
  include(${SWIG_USE_FILE})

  include_directories(${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR} ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${CLUCENE_CONFIG_INCLUDE_PATH})

  set_source_files_properties(libindex.i PROPERTIES CPLUSPLUS ON)
  if(HAVE_64_BIT)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -D64_BITS)
  endif(HAVE_64_BIT)

  swig_add_module(libindex python libindex.i index.cpp index_search.cpp)
  if(UNIX)
    swig_link_libraries(libindex ${PYTHON_LIBRARIES} ${QT_QTCORE_LIBRARIES} clucene _libvfs)
  else (UNIX)
    swig_link_libraries(libindex ${PYTHON_LIBRARIES} ${QT_QTCORE_LIBRARIES} clucene _libvfs)
  endif (UNIX)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties (${SWIG_MODULE_libindex_REAL_NAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
      RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
      SUFFIX ".pyd"
      )
  endif(CMAKE_GENERATOR MATCHES "Visual Studio")
  
  set(index_srcs
    __init__.py
    index.cpp
    index_search.cpp
    libindex.py
    )
  
  install_lib(${SWIG_MODULE_libindex_REAL_NAME})
  install_file(index ${index_srcs})
endif(EXISTS ${CLUCENE_CONFIG_INCLUDE_PATH} AND EXISTS ${CLUCENE_LIBRARY})
