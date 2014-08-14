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

namespace Destruct{
class DObject;
};

class Node;
class Constant;
typedef std::map<std::string, RCPtr< class Variant > > Attributes;

/**
 *   Class to inherit to process and return datatype
 */
class DataTypeHandler
{
public:
  EXPORT			                        DataTypeHandler(std::string);
  EXPORT  virtual 		                        ~DataTypeHandler();
  EXPORT  virtual std::string	                        type(Node* ) = 0;
  std::string			                        name;
}; 

/**
 *  A class for each type who contain type name, type compatible modules, and name of handler who generate it
 */
class DataType
{
public:
  EXPORT                                                DataType(DataTypeHandler* handler, const std::string name);
  EXPORT                                                DataType(DataTypeHandler* handler, const std::string name, std::list<std::string> const& compatibleModules);
  EXPORT                                                ~DataType();
  EXPORT const std::string                              handlerName(void) const;
  EXPORT const std::string                              name(void) const;
  EXPORT const std::list<std::string>                   compatibleModules(void) const;
  EXPORT bool                                           load(Destruct::RealValue<Destruct::DObject*>);
  EXPORT Destruct::RealValue<Destruct::DObject*>        save(void) const; 
private:
  void		                                        __compatibleModulesByType(const std::map<std::string, Constant*>& cmime, const std::string dtypes, std::list<std::string>& result);
  DataTypeHandler*                                      __handler;  //to serialize by string name
  const std::string                                     __name;   //to serialize by string name
  std::list<std::string>                                __compatibleModules; //to serialize by string list
};

/**
 *  A container for class Type ordered by typeName 
 */
class DataTypes
{
public:
  EXPORT                                                DataTypes();
  EXPORT                                                ~DataTypes();
  EXPORT const DataType*                                find(std::string typeName) const;
  EXPORT const DataType*                                insert(DataTypeHandler*, std::string typeName);
  EXPORT bool                                           load(Destruct::RealValue<Destruct::DObject*>);
  EXPORT Destruct::RealValue<Destruct::DObject*>        save(void) const;
private:
  std::map<const std::string, const DataType* >         __dataTypes; //to serialize by map ? string / dobject type
};

/**
 * A container to map node data to it's different compatible Type 
 */
class NodesTypes
{
public:
  EXPORT                                                NodesTypes();
  EXPORT const std::vector<const DataType* >            find(Node* node) const; 
  EXPORT void                                           insert(Node* node, const DataType* type);
  EXPORT bool                                           load(Destruct::RealValue<Destruct::DObject*>);
  EXPORT Destruct::RealValue<Destruct::DObject*>        save(void) const;
private:
  std::map<Node*, std::vector<const DataType* > >       __nodesDataTypes; //XXX dff:map //for locking / to serialize by map dobject, dobject (string, type) -> could be a list to insert in map ? 
};

/**
 *  This class let you register handler and get info for node compatible type and modules
 */
class DataTypeManager 
{
public:
  EXPORT static DataTypeManager* 		Get();
  EXPORT bool					registerHandler(DataTypeHandler* dataTypeHandler);
  EXPORT DataTypeHandler*                       handler(const std::string& name) const;
  EXPORT Attributes				type(Node* node);
  EXPORT std::list<std::string>                 compatibleModules(Node* node);

//** save & load method and helpers **//

  EXPORT Destruct::RealValue<Destruct::DObject*> nodeDataType(Node* node); //return only type if exist 
  EXPORT void                                   declare(void); //register needed dstruct
  EXPORT bool                                   load(Destruct::DValue value); //Destruct::DStream ?
  EXPORT void                                   loadNodeDataTypes(Node* node, Destruct::DValue const& value);
  EXPORT Destruct::DValue                       save(void) const;
private:
  EXPORT					DataTypeManager();
  						DataTypeManager(const DataTypeManager&);
  EXPORT					~DataTypeManager();
  DataTypeManager&				operator=(DataTypeManager& copy);
                                                mutex_def(__mutex);
  EXPORT const std::vector<const DataType*>     __dataType(Node* node);
  void	                                        __compatibleModulesByExtension(const std::map<std::string, Constant*>& cextensions, std::string& ext, std::list<std::string>& result);
  std::list<DataTypeHandler*>			__handlers; //not to serialize 
  NodesTypes                                    __nodesDataTypes; //map of node , vector<type> must serialize 
  DataTypes                                     __dataTypes;  //map of <string, Type*>  must serioalize
};

#endif
