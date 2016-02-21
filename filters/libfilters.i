/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2013 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http://www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Frederic B. <fba@digital-forensic.org>
 */

#include "pyrun.swg"

%module(package="dff.api.filters") libfilters

#ifndef WIN32
%include "stdint.i"
#else
%include "wstdint.i"
#endif

%include "std_string.i"
%include "std_map.i"
%include "std_vector.i"
%include "windows.i"

%import "../exceptions/libexceptions.i"

%{
#include "mfso.hpp"
#include "vfs.hpp"
#include "vlink.hpp"
#include "node.hpp"
#include "exceptions.hpp"
#include "datetime.hpp"
#include "rootnode.hpp"
#include "filter.hpp"
#include "timeline.hpp"
#include "dictionnary.hpp"
%}

%import "../vfs/libvfs.i"

namespace std
{
  %template(BadPatterns)            std::vector<BadPattern*>;
  %template(VectString)             std::vector<std::string>;
  %template(Dictionnaries)          std::map<std::string, Dictionnary* >;
  %template(VectTimeLineNode)       std::vector<TimeLineNode*>;
};

%traits_swigtype(Dictionnary);
%fragment(SWIG_Traits_frag(Dictionnary));


%include "../include/filter.hpp"
%include "../include/timeline.hpp"
%include "factory.hpp"
%include "dictionnary.hpp"
