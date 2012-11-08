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
#  Frederic Baguelin <fba@digital-forensic.org>
#  Solal Jacob <sja@digital-forensic.org>


FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#FIND_PACKAGE(PythonLibs)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(../include)

add_library(events SHARED eventhandler.cpp)
target_link_libraries(events types exceptions)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (events PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
  SUFFIX ".dll"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")


SET_SOURCE_FILES_PROPERTIES(libevents.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libevents python libevents.i)
SWIG_LINK_LIBRARIES(libevents ${PYTHON_LIBRARIES} events)
if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (${SWIG_MODULE_libevents_REAL_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
  SUFFIX ".pyd"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

if(UNIX)
 set_target_properties(events PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
 )
 set_target_properties(${SWIG_MODULE_libevents_REAL_NAME} PROPERTIES
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/"
  INSTALL_RPATH_USE_LINK_PATH TRUE
 )
endif(UNIX)

set(events_srcs
  __init__.py
  libevents.py
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

if(UNIX)
 INSTALL(TARGETS events DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
else(UNIX)
 install_lib(events)
endif(UNIX)
install_lib(${SWIG_MODULE_libevents_REAL_NAME})
install_file(events ${events_srcs})
