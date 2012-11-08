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

find_package(FLEX)


INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(../include)

# you can use -DAPI_FILTER_DEV to call flex and bison for each make

if (API_FILTER_DEV)

  find_package(BISON REQUIRED)
  BISON_TARGET(parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp)
  find_package(FLEX REQUIRED)
  FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp)

else (API_FILTER_DEV)

  if ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp )
    message(STATUS "bison files NOT generated, Bison executable is required")
    find_package(BISON REQUIRED)
    BISON_TARGET(parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp)
  else ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp )
    set(BISON_parser_OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp)
    message(STATUS "BISON files already generated and setted to ${CMAKE_CURRENT_SOURCE_DIR}/parser.c") 
  endif ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp )

  if ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp )
    message(STATUS "flex files NOT generated, Flex executable is required")
    find_package(FLEX REQUIRED)
    FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp)
  else (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp )
    message(STATUS "FLEX files already generated and setted to ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c")
    set(FLEX_scanner_OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp)
  endif ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp )

endif (API_FILTER_DEV)


message(STATUS ${FLEX_scanner_OUTPUTS} ${BISON_parser_OUTPUTS})

# does not seem to be mandatory. Here just in case and as a reminder...
#ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_library(filters SHARED factory.cpp dictionnary.cpp filter.cpp astnodes.cpp ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS})
target_link_libraries(filters events vfs types search)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties (filters PROPERTIES
  SUFFIX ".dll"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")


SET_SOURCE_FILES_PROPERTIES(libfilters.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libfilters python libfilters.i)

SWIG_LINK_LIBRARIES(libfilters ${PYTHON_LIBRARIES} filters)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties (${SWIG_MODULE_libfilters_REAL_NAME} PROPERTIES
  SUFFIX ".pyd"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

if(UNIX)
 set_target_properties(filters PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
 )
 set_target_properties(${SWIG_MODULE_libfilters_REAL_NAME} PROPERTIES
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/"
  INSTALL_RPATH_USE_LINK_PATH TRUE
 )
endif(UNIX)

set(libfilters_srcs
  libfilters.py
  __init__.py
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

if(UNIX)
 INSTALL(TARGETS filters DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
else(UNIX)
 install_lib(filters)
endif(UNIX)
install_lib(${SWIG_MODULE_libfilters_REAL_NAME})
install_file(libfilters ${libfilters_srcs})
