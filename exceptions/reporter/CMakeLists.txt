# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2015 ArxSys
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

cmake_minimum_required (VERSION 2.6)

project (CrashReporter)

set (CrashReporter_VERSION_MAJOR 1)
set (CrashReporter_VERSION_MINOR 0)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt4 REQUIRED QtCore QtGui)

if (!QT4_FOUND)
   message(error "Qt Not found!")
endif()

qt4_wrap_cpp(CrashDialog_MOC crashdialog.hpp)

include(${QT_USE_FILE})
include_directories("${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad")

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(CrashReporter main.cpp crashdumpinfo.cpp crashreporter.cpp crashdialog.cpp ${CrashDialog_MOC} ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/common/linux/libcurl_wrapper.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/common/linux/google_crashdump_uploader.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/minidump.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/logging.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/dump_context.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/dump_object.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/basic_code_modules.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/pathname_stripper.cc ${CMAKE_SOURCE_DIR}/dff/api/exceptions/breakpad/processor/symbolic_constants_win.cc)

target_link_libraries (CrashReporter Qt4::QtCore Qt4::QtGui dl)

install (TARGETS CrashReporter DESTINATION bin)
