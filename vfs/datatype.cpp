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

#include "vfile.hpp"
#include "../magic/magic.h"
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
 *  DataType
 */ 
DataType::DataType(const std::string name) : __name(name)
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

DataType::DataType(const std::string name, const std::list<std::string>& compatibleModule) : __name(name), __compatibleModules(compatibleModule)
{
}

DataType::~DataType()
{
}

const std::string       DataType::name() const
{
  return (this->__name);
}

const std::list<std::string>    DataType::compatibleModules(void) const
{
  return (this->__compatibleModules);
}

void		DataType::__compatibleModulesByType(const std::map<std::string, Constant*>& cmime, const std::string dtypes, std::list<std::string>& result)
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

DataType*                                       DataType::load(DValue const&  dobject)
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

  DataType* type = new DataType(name, compatibleModule);
  return (type);
}                       

DValue                DataType::save(void) const
{
  DObject* dataType = DStructs::instance().generate("DataType");

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
  std::map<const std::string, const DataType*>::iterator type = this->__types.begin();

  for (; type != this->__types.end(); ++type)
    delete (*type).second;
  this->__types.clear();
  this->__nodesType.clear();
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
  
  if (node != NULL)
  {  
    // At first, check if node's type has already been processed
    mutex_lock(&this->__mutex);
    std::map<Node*, const DataType* >::const_iterator nodeType = this->__nodesType.find(node);
    mutex_unlock(&this->__mutex);
    if (nodeType != this->__nodesType.end())
    {
      if (nodeType->second != NULL)
	return (nodeType->second->name());
    }
    // else, process node's type and return it;
    else
    {
      std::string result;
      if (node->size() > 0)
      {
         magic_t mime = magic_open(MAGIC_NONE);
         magic_load(mime, "dff/api/magic/magic.mgc");
         try
         {
           VFile* vfile = node->open();
           uint8_t buff[0x2000];
           uint32_t size = vfile->read(&buff, 0x2000);
           vfile->close();

           const char* magic_result = magic_buffer(mime, &buff, size);
           if (magic_result)
             result = std::string(magic_result);
           else
             result = "data";
           magic_close(mime);
         }
         catch (...)
         {
           result = "empty";
         }
      }
      else if (node->hasChildren())
        result = "directory";
      else
        result = "empty";  
 
      mutex_lock(&this->__mutex);
      std::map<const std::string, const DataType* >::const_iterator types = this->__types.find(result);
      if (types == this->__types.end())
      {
         DataType*	type = new DataType(result);
	 this->__types[result] = type;
         this->__nodesType[node] = type;
      }
      else
        this->__nodesType[node] = types->second;
      mutex_unlock(&this->__mutex);
      return (result);
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
  const DataType* type = NULL;

  mutex_lock(&this->__mutex);
  std::map<const std::string, const DataType* >::const_iterator types = this->__types.find(dtype);
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

bool                    DataTypeManager::loadNodesType(Node* node, Destruct::DValue const& value)
{
  this->__nodesType[node] = this->__types[value.get<DUnicodeString>()];
  return (true);
}

bool                    DataTypeManager::load(Destruct::DValue value)
{
  Destruct::DObject* types = value;

  DUInt64 typesCount = types->call("size");
  for (DUInt64 index = 0; index < typesCount; index++)
  {
     Destruct::DValue typeObject = types->call("get", Destruct::RealValue<DUInt64>(index));
     DataType* type = DataType::load(typeObject);
     if (type)
       this->__types[type->name()] = type;
     else
        std::cout << "DataType::load return NULL " << std::endl;
  }
  types->destroy();

  return (true);
}

Destruct::DValue        DataTypeManager::save(void) const
{
  Destruct::DObject* types  = Destruct::DStructs::instance().find("DVectorObject")->newObject();

  std::map<const std::string, const DataType*>::const_iterator typeIt = this->__types.begin();
  for (; typeIt != this->__types.end(); ++typeIt)
  {
    if ((*typeIt).second != NULL)
      types->call("push", (*typeIt).second->save());
    else
      std::cout << "Null type in DataTypeManager::__types " << std::endl;
  }

  return (RealValue<DObject*>(types));
}

}
