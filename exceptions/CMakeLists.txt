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
#FIND_PACKAGE(PythonInterp)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(../include)

add_library(exceptions SHARED exceptions.cpp)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (exceptions PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
  SUFFIX ".dll"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

SET_SOURCE_FILES_PROPERTIES(libexceptions.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(libexceptions python libexceptions.i)
SWIG_LINK_LIBRARIES(libexceptions ${PYTHON_LIBRARIES} exceptions)

if ( CMAKE_GENERATOR MATCHES "Visual Studio")
set_target_properties (${SWIG_MODULE_libexceptions_REAL_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
  SUFFIX ".pyd"
)
endif ( CMAKE_GENERATOR MATCHES "Visual Studio")

if(UNIX)
 set_target_properties(exceptions PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
 )
 set_target_properties(${SWIG_MODULE_libexceptions_REAL_NAME} PROPERTIES
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/"
  INSTALL_RPATH_USE_LINK_PATH TRUE
 )
endif(UNIX)

if (UNIX)
add_custom_target(patchexceptions ALL
  DEPENDS ${SWIG_MODULE_libexceptions_REAL_NAME}
  COMMENT "Patching libexceptions.py"
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/patch.py
    ${CMAKE_CURRENT_BINARY_DIR}/libexceptions.py
)
elseif(UNIX)
add_custom_target(patchexceptions ALL
  DEPENDS ${SWIG_MODULE_libexceptions_REAL_NAME}
  COMMENT "Patching libexceptions.py"
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/patch.py
    ${CMAKE_CURRENT_BINARY_DIR}/libexceptions.py
)
endif(UNIX)
set(exceptions_srcs
  __init__.py
   libexceptions.py
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

install_lib(${SWIG_MODULE_libexceptions_REAL_NAME})
if(UNIX)
 INSTALL(TARGETS exceptions DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
else(UNIX)
 install_lib(exceptions)
endif(UNIX)
install_file(exceptions ${exceptions_srcs})
