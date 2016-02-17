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

namespace DFF
{

/**
 *  Type
 */ 
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

/**
 *  Type
 */
Type::Type(DataTypeHandler* handler, const std::string name) : __handler(handler), __name(name)
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

Type::~Type()
{
}

const std::string               Type::handlerName(void) const
{
  return (this->__handler->name);
}

const std::string       Type::name() const
{
  return (this->__name);
}

const std::list<std::string>    Type::compatibleModules(void) const
{
  return (this->__compatibleModules);
}

/** 
 * Types
 */
Types::Types()
{
}

Types::~Types()
{
  std::map<const std::string, const Type* >::iterator type = this->__types.begin();

  for (; type != this->__types.end(); ++type)
    delete (type->second);
}

const Type*     Types::find(std::string typeName) const
{
  std::map<const std::string, const Type* >::const_iterator type = this->__types.find(typeName);
  if (type != this->__types.end())
    return (type->second);
  return (NULL);
}  

const Type*     Types::insert(DataTypeHandler* handler, std::string typeName)
{
  const Type* newType = new Type(handler, typeName);
  this->__types[typeName] = newType;

  return (newType);
}

/**
 *  NodesTypes
 */
NodesTypes::NodesTypes()
{
}

const std::vector<const Type* > NodesTypes::find(Node* node) const
{
  std::vector<const Type* >     types;
  std::map<Node*, std::vector<const Type*> >::const_iterator nodeTypes = this->__nodesTypes.find(node);
  if (nodeTypes != this->__nodesTypes.end())
    return (nodeTypes->second);

  return (types);
}

void    NodesTypes::insert(Node* node, const Type* type)
{
 this->__nodesTypes[node].push_back(type);
}

/**
 *  DataTypeManager
 */
DataTypeManager::DataTypeManager()
{
  mutex_init(&this->__mutex);
}

DataTypeManager* 	DataTypeManager::Get()
{
  static DataTypeManager single;
  return &single;
}

DataTypeManager::~DataTypeManager()
{
  mutex_destroy(&this->__mutex);
}

bool		DataTypeManager::registerHandler(DataTypeHandler* handler)
{
  this->__handlers.push_back(handler);
  return true;
}

Attributes	DataTypeManager::type(Node* node)
{
  Attributes					attributes;
  const std::vector<const Type*> types = this->__type(node);
  
  if (types.size())
  {
    std::vector<const Type*>::const_iterator type = types.begin();
    for (; type != types.end(); ++type)
    { 
      if (*type) //should never be null but .. 
        attributes[(*type)->handlerName()] = new Variant((*type)->name());
    }
  }
  return (attributes);
}

const std::vector<const Type*>  DataTypeManager::__type(Node* node)
{
  std::vector<const Type*> types; 
  std::list<DataTypeHandler* >::iterator	handler;
  
  if (node == NULL)
    return (types);

  mutex_lock(&this->__mutex);
  types = this->__nodesTypes.find(node);
  mutex_unlock(&this->__mutex);
  if (types.size())
    return (types);
  else
  {
    for (handler = this->__handlers.begin(); handler != this->__handlers.end(); handler++)
    {
      std::string result = "error";
      try 
      {
        result = (*handler)->type(node);
      }
      catch (...)
      {
        //std::cout << "Error : Can't get data type on node"  << std::endl;
        result = "error";
      }
      mutex_lock(&this->__mutex);
      const Type* type = this->__types.find(result); //find for handler and insert if not coun
      if (type == NULL)
        type = this->__types.insert(*handler, result);
      this->__nodesTypes.insert(node, type);
      mutex_unlock(&this->__mutex);
    } 
  }
  mutex_lock(&this->__mutex);
  types = this->__nodesTypes.find(node);
  mutex_unlock(&this->__mutex);
  return (types);
}

std::list<std::string>  DataTypeManager::compatibleModules(Node* node)
{
  std::list<std::string>   modules;
  Attributes attributes = node->dataType(); //node dataType could be overloaded so must call it
  Attributes::iterator attribute = attributes.begin(); 

  for (; attribute != attributes.end(); ++attribute)
  {
    std::string typeName = attribute->second->value<std::string>();
    mutex_lock(&this->__mutex);
    const Type* type = this->__types.find(typeName);
    mutex_unlock(&this->__mutex);
    if (type == NULL)
      continue ;
    
    //modules.copy() 
    std::list<std::string> currentModules = type->compatibleModules();
    std::list<std::string>::iterator currentModule = currentModules.begin();
    for (; currentModule != currentModules.end(); ++currentModule)
      modules.push_back(*currentModule);
  }
  modules.unique();

  return (modules);
}

/**
 *  DataTypeHandler
 */
DataTypeHandler::DataTypeHandler(std::string handlerName)
{
  DataTypeManager* 	dataTypeManager =  DataTypeManager::Get();

  this->name = handlerName;
  dataTypeManager->registerHandler(this);
}

DataTypeHandler::~DataTypeHandler()
{

}

}
