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

#ifndef __DATATYPE_HPP__
#define __DATATYPE_HPP__

#ifndef WIN32
  #include <stdint.h>
#elif _MSC_VER >= 1600
  #include <stdint.h>
#else
  #include "wstdint.h"
#endif
#include <vector>
#include <map>
#include <list>
#include <stdexcept>
#include <string>

#include "rc.hpp"
#include "variant.hpp"
#include "threading.hpp"

#include "drealvalue.hpp"

using namespace Destruct;

namespace DFF
{
class Node;
class Constant;

/**
 *   Class to inherit to process and return datatype
 */
class DataTypeHandler
{
public:
  EXPORT			                        DataTypeHandler();
  EXPORT  virtual 		                        ~DataTypeHandler();
  EXPORT  virtual std::string	                        type(Node* ) = 0;
}; 

class DataType //dccpobject ? 
{
public:
  EXPORT DataType(const std::string name);
  EXPORT DataType(const std::string name, const std::list<std::string>& compatibleModule); 
  EXPORT ~DataType();

  EXPORT const std::string		                name(void) const;
  EXPORT const std::list<std::string>	                compatibleModules(void) const; 
  EXPORT static DataType*                               load(DValue const& value);
  EXPORT DValue                                         save(void) const; 
private:
  const std::string			                __name;
  std::list<std::string>		                __compatibleModules;
  void					                __compatibleModulesByType(const std::map<std::string, Constant*>& cmime, const std::string dtypes, std::list<std::string>& result);
};

/**
 *  This class let you register handler and get info for node compatible type and modules
 */
class DataTypeManager  //DCppObjectSingleton ? 
{
public:
  EXPORT static DataTypeManager* 		Get();
  EXPORT bool					registerHandler(DataTypeHandler* dataTypeHandler);
  EXPORT const std::string			type(Node* node); //typeId system better ?
  EXPORT std::list<std::string>                 compatibleModules(Node* node);

  //EXPORT DataTypeHandler*                       handler(const std::string& name) const;
  EXPORT void                                   declare(void);
  EXPORT bool                                   loadNodesType(Node* node, DValue const& value);
  EXPORT bool                                   load(DValue value); //Destruct::DStream ?
  EXPORT DValue                                 save(void) const;
private:
  EXPORT					DataTypeManager();
  						DataTypeManager(const DataTypeManager&);
  EXPORT					~DataTypeManager();
  DataTypeManager&				operator=(DataTypeManager& copy);
                                                mutex_def(__mutex);
  DataTypeHandler*				__handler;
  std::map<const std::string, const DataType* >	__types;
  std::map<Node*, const DataType* >		__nodesType;
  //type std::list<Node*> ?
  void		                                __compatibleModulesByExtension(const std::map<std::string, Constant*>& cextensions, std::string& ext, std::list<std::string>& result);
};

}
#endif
