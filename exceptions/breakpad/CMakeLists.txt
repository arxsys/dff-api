# Copyright (C) 2013-2014 ArxSys
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

#set(CMAKE_INCLUDE_PATH "${INCLUDEDIR}")
#set(CMAKE_LIBRARY_PATH "${LIBDIR}")

cmake_minimum_required(VERSION 2.8.12)

include_directories(${CMAKE_CURRENT_SOURCE_DIRECTORY})

if (WIN32)

  set(breakpad_client_sources    
    client/windows/crash_generation/crash_generation_client.cc
    client/windows/crash_generation/crash_generation_server.cc
    client/windows/crash_generation/client_info.cc
    client/windows/crash_generation/minidump_generator.cc
    client/windows/handler/exception_handler.cc
    client/windows/sender/crash_report_sender.cc
    common/windows/guid_string.cc
    common/windows/http_upload.cc
    common/windows/string_utils.cc
    )

endif (WIN32)

if (UNIX)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=missing-braces -Werror=non-virtual-dtor -Werror=overloaded-virtual -Werror=reorder -Werror=sign-compare -Werror=unused-variable -Werror=vla")

  set(breakpad_client_sources
    client/linux/crash_generation/crash_generation_client.cc
    client/linux/crash_generation/crash_generation_server.cc
    client/linux/dump_writer_common/seccomp_unwinder.cc
    client/linux/dump_writer_common/thread_info.cc
    client/linux/dump_writer_common/ucontext_reader.cc
    client/linux/handler/exception_handler.cc
    client/linux/handler/minidump_descriptor.cc
    client/linux/log/log.cc
    client/linux/microdump_writer/microdump_writer.cc
    client/linux/minidump_writer/linux_dumper.cc
    client/linux/minidump_writer/linux_ptrace_dumper.cc
    client/linux/minidump_writer/minidump_writer.cc
    client/minidump_file_writer.cc
    common/convert_UTF.c
    common/md5.cc
    common/string_conversion.cc
    common/linux/elfutils.cc
    common/linux/file_id.cc
    common/linux/guid_creator.cc
    common/linux/linux_libc_support.cc
    common/linux/memory_mapped_file.cc
    common/linux/safe_readlink.cc
    )

endif(UNIX)

add_library(breakpad_client STATIC ${breakpad_client_sources})

if ( CMAKE_GENERATOR MATCHES "Visual Studio" )
  set_target_properties (breakpad_client PROPERTIES
    SUFFIX ".lib"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
    )
endif()
