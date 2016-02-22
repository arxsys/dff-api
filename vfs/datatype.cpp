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
#include "exceptions.hpp"
#include "variant.hpp"
#include "node.hpp"
#include "datatype.hpp"
#include "config.hpp"
#include "confmanager.hpp"
#include "node.hpp"
#include "vfs.hpp"

#include "dstructs.hpp"
#include "dsimpleobject.hpp"
#include "dobject.hpp"
#include "dnullobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

#include "dtype.hpp" //XXX TEMPORARY FOR TYPE INIT MUST BE DONE ELSEWHERE !


namespace DFF
{
/**
 *  DataTypeHandler
 */
DataTypeHandler::DataTypeHandler()
{
  DataTypeManager* 	dataTypeManager = DataTypeManager::Get();

  dataTypeManager->registerHandler(this);
}

DataTypeHandler::~DataTypeHandler()
{

}
/**
 *  Type
 */ 
Type::Type(const std::string name) : __name(name)
{
  std::list<std::string>        	result;
  ConfigManager*			cm;
  std::map<std::string, Constant*>	constants;
  std::string				ext;

  if ((cm = ConfigManager::Get()) != NULL)
    {
      constants = cm->constantsByName("mime-type");
      if (!constants.empty())       
	{
	  this->__compatibleModulesByType(constants, name, result);
	  this->__compatibleModules = result;
	}
    }
}

Type::Type(const std::string name, const std::list<std::string>& compatibleModule) : __name(name), __compatibleModules(compatibleModule)
{
}

Type::~Type()
{
}

const std::string       Type::name() const
{
  return (this->__name);
}

const std::list<std::string>    Type::compatibleModules(void) const
{
  return (this->__compatibleModules);
}

void		Type::__compatibleModulesByType(const std::map<std::string, Constant*>& cmime, const std::string dtypes, std::list<std::string>& result)
{
  std::map<std::string, Constant*>::const_iterator	cit;
  std::list<Variant_p >					lvalues;
  std::list<Variant_p >::iterator			lit;
  bool							match;

  for (cit = cmime.begin(); cit != cmime.end(); cit++)
    {
      match = false;
      if ((cit->second != NULL) && (cit->second->type() == typeId::String))
	{
	  lvalues = cit->second->values();
	  lit = lvalues.begin();
	  while (lit != lvalues.end() && !match)
	    {
	      std::string	cval = (*lit)->value<std::string>();
	      if (dtypes.find(cval) != std::string::npos)
		{
		  match = true;
		  result.push_back(cit->first);
		}
	      lit++;
	    }
	}
    }
}

Type*                                       Type::load(DValue const&  dobject)
{
  DObject* typeObject = dobject.get<DObject*>();

  std::list<std::string> compatibleModule;
  std::string name = typeObject->getValue("name").get<DUnicodeString>();
 
  DObject* compatibleModulesObject = typeObject->getValue("compatibleModules").get<DObject*>();
  DUInt64  compatibleModulesCount = compatibleModulesObject->call("size").get<DUInt64>();

  for (DUInt64 index = 0; index < compatibleModulesCount; index++)
  {
     std::string module = compatibleModulesObject->call("get", RealValue<DUInt64>(index)).get<DUnicodeString>();
     compatibleModule.push_back(module);
  }

  compatibleModulesObject->destroy();
  typeObject->destroy();

  Type* type = new Type(name, compatibleModule);
  return (type);
}                       

DValue                Type::save(void) const
{
  DObject* dataType = DStructs::instance().generate("Type");

  dataType->setValue("name", RealValue<DUnicodeString>(this->__name));

  DObject* dvectorString = DStructs::instance().generate("DVectorString");
  std::list<std::string>::const_iterator compatibleModule = this->__compatibleModules.begin();
  for (; compatibleModule != this->__compatibleModules.end(); compatibleModule++)
    dvectorString->call("push", RealValue<DUnicodeString>(*compatibleModule));

  dataType->setValue("compatibleModules", RealValue<DObject*>(dvectorString));
  dvectorString->destroy();

  return (RealValue<DObject*>(dataType));
}

/**
 *  DataTypeManager
 */
DataTypeManager::DataTypeManager()
{
  mutex_init(&this->__mutex);
  this->declare();
}

/**
 *  Return DataTypeManager singleton instance
 */
DataTypeManager* 	DataTypeManager::Get()
{
  static DataTypeManager single;
  return &single;
}

DataTypeManager::~DataTypeManager()
{
  mutex_destroy(&this->__mutex);
}

/**
 *  Register a new datatype handler
 */
bool		DataTypeManager::registerHandler(DataTypeHandler* handler)
{
  if (this->__handler != NULL)
    delete this->__handler;
  this->__handler = handler;
  return true;
}

/**
 *  Search for Node type and return it as string
 */
const std::string	DataTypeManager::type(Node* node)
{
  const	std::string	dtype;
  const Type*		type;
  
  if (node != NULL)
    {  
      // At first, check if node's type has already been processed
      mutex_lock(&this->__mutex);
      std::map<Node*, const Type* >::const_iterator nodeType = this->__nodesType.find(node);
      mutex_unlock(&this->__mutex);
      if (nodeType != this->__nodesType.end())
	{
	  if ((type = nodeType->second) != NULL)
	    return type->name();
	}
      // else, process node's type and return it;
      else
	{
	  std::string result;
	  try
	    {
	      result = this->__handler->type(node);
	    }
	  catch (...)
	    {
	      result = std::string("error");
	    }
	  mutex_lock(&this->__mutex);
	  std::map<const std::string, const Type* >::const_iterator types = this->__types.find(result);
	  if (types == this->__types.end())
	    {
	      type = new Type(result);
	      this->__types[result] = type;
	    }
	  else
	    type = types->second;
	  this->__nodesType[node] = type;
	  mutex_unlock(&this->__mutex);
	  return result;
	}
    }
  return std::string("");
}

/**
 *  Return list of compatible modules for Node passed as argument
 */
std::list<std::string>		DataTypeManager::compatibleModules(Node* node)
{
  std::list<std::string>	modules;
  const std::string dtype = node->dataType(); //node dataType could be overloaded so must call it
  const Type* type = NULL;

  mutex_lock(&this->__mutex);
  std::map<const std::string, const Type* >::const_iterator types = this->__types.find(dtype);
  mutex_unlock(&this->__mutex);
  if (types != this->__types.end())
    type = types->second;
  if (type != NULL)
    {
      //modules.copy()
      std::list<std::string> currentModules = type->compatibleModules();
      std::list<std::string>::iterator currentModule = currentModules.begin();
      for (; currentModule != currentModules.end(); ++currentModule)
  	modules.push_back(*currentModule);
      modules.unique();
    }
  return (modules);
}

void                    DataTypeManager::declare(void)
{
  Destruct::DType::init(); //XXX for test only must be done elswhere

  Destruct::DStructs&    destruct = Destruct::DStructs::instance();
  Destruct::DStruct* dtype = new Destruct::DStruct(NULL, "Type", Destruct::DSimpleObject::newObject); 
  dtype->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"));
  dtype->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "compatibleModules"));
  destruct.registerDStruct(dtype);
}

bool                    DataTypeManager::loadNodesType(Node* node, Destruct::DValue const& value)
{
  this->__nodesType[node] = this->__types[value.get<DUnicodeString>()];
  return (true);
}

bool                    DataTypeManager::load(Destruct::DValue value)
{
  Destruct::DObject* types = value.get<DObject*>();

  DUInt64 typesCount = types->call("size").get<DUInt64>();
  for (DUInt64 index = 0; index < typesCount; index++)
  {
     Destruct::DValue typeObject = types->call("get", Destruct::RealValue<DUInt64>(index));
     Type* type = Type::load(typeObject);
     this->__types[type->name()] = type;
  }
  types->destroy();

  return (true);
}

Destruct::DValue        DataTypeManager::save(void) const
{
  Destruct::DObject* types  = Destruct::DStructs::instance().find("DVectorObject")->newObject();

  std::map<const std::string, const Type*>::const_iterator typeIt = this->__types.begin();
  for (; typeIt != this->__types.end(); ++typeIt)
    types->call("push", (*typeIt).second->save());

  return (RealValue<DObject*>(types));
}

}
