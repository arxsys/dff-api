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

include_directories(${PYTHON_INCLUDE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_HOME_DIRECTORY}/dff/api/include)
include_directories(${CMAKE_HOME_DIRECTORY}/dff/api/tree)
include_directories(${ICU_INCLUDE_DIRS})

include_directories(${FFMPEG_INCLUDE_DIRS})

set(video_srcs
  video.cpp
)

MESSAGE( STATUS "Libs ffmpeg : ${FFMPEG_LIBRARIES} ${FFMPEG_LIBSWSCALE_LIBRARIES}")
add_library(video SHARED ${video_srcs})
target_link_libraries(video exceptions types vfs ${FFMPEG_LIBRARIES})

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (video PROPERTIES
  SUFFIX ".dll"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

SET_SOURCE_FILES_PROPERTIES(video.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(video python video.i)
SWIG_LINK_LIBRARIES(video ${PYTHON_LIBRARIES} video)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (${SWIG_MODULE_video_REAL_NAME} PROPERTIES
  SUFFIX ".pyd"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

if(UNIX)
 set_target_properties(video PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
 )
 set_target_properties(${SWIG_MODULE_video_REAL_NAME} PROPERTIES
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/"
  INSTALL_RPATH_USE_LINK_PATH TRUE
 )
endif(UNIX)

set(video_files
  __init__.py
  video.py
)

install_lib(${SWIG_MODULE_video_REAL_NAME})
if(UNIX)
 INSTALL(TARGETS video DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
else(UNIX)
 install_lib(video)
endif(UNIX)
install_file(videogui ${video_files})
