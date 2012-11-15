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
#  Solal Jacob <sja@digital-forensic.org>

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#FIND_PACKAGE(PythonLibs)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(../include)

if (UNIX)
       if (HAVE_UDEV)
	 add_definitions(-DHAVE_LIBUDEV_H)
	 set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DHAVE_LIBUDEV_H=1)
  	 set (libdevices_srcs
       	    device.cpp
       	    udevice.cpp
            )
       else (HAVE_UDEV)
         set (libdevices_srcs
	     device.cpp
	    )
       endif (HAVE_UDEV)
else (UNIX)
   set (libdevices_srcs
       device.cpp
       wmidev.cpp	
     )
endif(UNIX)

SET_SOURCE_FILES_PROPERTIES(libdevices.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libdevices python libdevices.i ${libdevices_srcs})

if (HAVE_UDEV)
SWIG_LINK_LIBRARIES(libdevices ${PYTHON_LIBRARIES} ${UDEV_LIBRARY})
else (HAVE_UDEV) 
SWIG_LINK_LIBRARIES(libdevices ${PYTHON_LIBRARIES})
endif (HAVE_UDEV)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (${SWIG_MODULE_libdevices_REAL_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
  SUFFIX ".pyd"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")
if (UNIX)
set(libdevices_files
  __init__.py
  libdevices.py
  devices.py
  dbushaldev.py
)
else (UNIX)
set(libdevices_files
  __init__.py
  libdevices.py
  devices.py
)
endif(UNIX)

install_lib(${SWIG_MODULE_libdevices_REAL_NAME})
install_file(libdevices ${libdevices_files})

