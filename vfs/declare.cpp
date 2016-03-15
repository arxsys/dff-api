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
 *  Solal J. <sja@digital-forensic.org>
 */

#include "dsimpleobject.hpp"
#include "dstruct.hpp"
#include "dstructs.hpp"

#include "vfs.hpp"
#include "node.hpp"

using namespace Destruct;

namespace DFF
{

void    VFS::declare(void)
{
  DStructs& destruct = DStructs::instance();

  DStruct*  dnodeStruct = new DStruct(0, "DNode", DSimpleObject::newObject); // rename NodeAttribute (on l'apply apres mais ca cree pas vraiment une node 
  dnodeStruct->addAttribute(DAttribute(DType::DUInt64Type, "uid"));
  dnodeStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "absolute")); //? 
  dnodeStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "name")); 
  dnodeStruct->addAttribute(DAttribute(DType::DObjectType, "tags")); 
  dnodeStruct->addAttribute(DAttribute(DType::DObjectType, "children")); 
  dnodeStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "dataType"));
  destruct.registerDStruct(dnodeStruct);

  destruct.registerDStruct(makeNewDCpp<NodeContainer>("NodeContainer"));

//XXX doublon avec ce ux method load pour ntfs 
  DStruct*  vlinkStruct = new DStruct(0, "VLink", DSimpleObject::newObject);
  vlinkStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "linkedNode"));
  vlinkStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "name"));
  destruct.registerDStruct(vlinkStruct);

  DStruct*  dpathStruct = new DStruct(0, "DPath", DSimpleObject::newObject);
  dpathStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "path")); 
  destruct.registerDStruct(dpathStruct);

  destruct.registerDStruct(makeNewDCpp<VoidNode>("VoidNode"));

  DStruct* dvlinkStruct = new DStruct(0, "DVLink", DSimpleObject::newObject);
  dvlinkStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "name"));
  dvlinkStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "node"));
  dvlinkStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "linkNode"));
  destruct.registerDStruct(dvlinkStruct);

  //destruct.registerDStruct(makeNewDCppSingleton<Time>("Time"));
  //destruct.registerDStruct(makeNewDCppSingleton<Log>("Log"));

  DStruct*  color = new DStruct(0, "Color", DSimpleObject::newObject);
  color->addAttribute(DAttribute(DType::DUInt8Type, "r"));
  color->addAttribute(DAttribute(DType::DUInt8Type, "g"));
  color->addAttribute(DAttribute(DType::DUInt8Type, "b"));
  destruct.registerDStruct(color);

  DStruct*  tag = new DStruct(0, "Tag", DSimpleObject::newObject);
  tag->addAttribute(DAttribute(DType::DUnicodeStringType, "name"));
  tag->addAttribute(DAttribute(DType::DObjectType, "color"));
  destruct.registerDStruct(tag);

  DStruct* dtype = new DStruct(NULL, "DataType", DSimpleObject::newObject); 
  dtype->addAttribute(DAttribute(DType::DUnicodeStringType, "name"));
  dtype->addAttribute(DAttribute(DType::DObjectType, "compatibleModules"));
  destruct.registerDStruct(dtype);
}

}
