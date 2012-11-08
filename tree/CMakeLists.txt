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


FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

#FIND_PACKAGE(PythonLibs)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(../include)

add_library(tree SHARED TwoThreeTree.cpp TwoThreeNode.cpp)
if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (tree PROPERTIES
  SUFFIX ".dll"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

SET_SOURCE_FILES_PROPERTIES(libtree.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libtree python libtree.i)
SWIG_LINK_LIBRARIES(libtree ${PYTHON_LIBRARIES} tree)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (${SWIG_MODULE_libtree_REAL_NAME} PROPERTIES
  SUFFIX ".pyd"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

if(UNIX)
 set_target_properties(tree PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
 )
 set_target_properties(${SWIG_MODULE_libtree_REAL_NAME} PROPERTIES
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/"
  INSTALL_RPATH_USE_LINK_PATH TRUE
 )
endif(UNIX)

set(tree_srcs
  libtree.py
  __init__.py
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

install_lib(${SWIG_MODULE_libtree_REAL_NAME})
if(UNIX)
 INSTALL(TARGETS tree DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
else(UNIX)
 install_lib(tree)
endif(UNIX)
install_file(tree ${tree_srcs})
