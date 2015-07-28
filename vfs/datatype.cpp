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

/**
 *  DataTypeHandler
 */
DataTypeHandler::DataTypeHandler(std::string handlerName)
{
  DataTypeManager* 	dataTypeManager = DataTypeManager::Get();

  this->name = handlerName;
  dataTypeManager->registerHandler(this);
}

DataTypeHandler::~DataTypeHandler()
{

}

/**
 *  DataType
 */
DataType::DataType(DataTypeHandler* handler, const std::string name) : __handler(handler), __name(name)
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

DataType::DataType(DataTypeHandler* handler, const std::string name, std::list<std::string> const& compatibleModules) : __handler(handler), __name(name), __compatibleModules(compatibleModules)
{
}



DataType::~DataType()
{
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

const std::string                            DataType::handlerName(void) const
{
  return (this->__handler->name);
}

const std::string                            DataType::name() const
{
  return (this->__name);
}

const std::list<std::string>                 DataType::compatibleModules(void) const
{
  return (this->__compatibleModules);
}

bool                                         DataType::load(Destruct::DValue const&  dobject)
{
  //XXX code me 
  return (false);
}                       

Destruct::RealValue<Destruct::DObject*>      DataType::save(void) const //return Destruct::RealValue //ref count ?
{
  Destruct::DObject* dataType = Destruct::DStructs::instance().generate("DataType"); //optimize keep dstruct pointer ? 

  dataType->setValue("handler",  Destruct::RealValue<Destruct::DUnicodeString>(this->__handler->name));
  dataType->setValue("name", Destruct::RealValue<Destruct::DUnicodeString>(this->__name));

  Destruct::DObject* dvectorString = Destruct::DStructs::instance().generate("DVectorString");
  std::list<std::string>::const_iterator compatibleModule = this->__compatibleModules.begin();
  for (; compatibleModule != this->__compatibleModules.end(); compatibleModule++)
    dvectorString->call("push", Destruct::RealValue<Destruct::DUnicodeString>(*compatibleModule));

  dataType->setValue("compatibleModules", Destruct::RealValue<Destruct::DObject*>(dvectorString));
  dvectorString->destroy();

  return (dataType);
}

/** 
 * Types
 */
DataTypes::DataTypes()
{
}

DataTypes::~DataTypes()
{
  std::map<const std::string, const DataType* >::iterator type = this->__dataTypes.begin();

  for (; type != this->__dataTypes.end(); ++type)
    delete (type->second);
}

const DataType*             DataTypes::find(std::string typeName) const
{
  std::map<const std::string, const DataType* >::const_iterator type = this->__dataTypes.find(typeName);
  if (type != this->__dataTypes.end())
    return (type->second);
  return (NULL);
}  

const DataType*                 DataTypes::insert(DataTypeHandler* handler, std::string typeName)
{
  const DataType* newDataType = new DataType(handler, typeName);
  this->__dataTypes[typeName] = newDataType;

  return (newDataType);
}

bool                            DataTypes::load(Destruct::DValue const&  dobject)
{
  Destruct::DObject* vector = dobject.get<Destruct::DObject*>();
  DataTypeManager* dataTypeManager = DataTypeManager::Get();

  DUInt64 size = vector->call("size").get<DUInt64>();
  for (DUInt64 i = 0; i < size; i++) 
  {
    Destruct::DObject* dataType = vector->call("get", Destruct::RealValue<DUInt64>(i)).get<Destruct::DObject*>();

    std::list<std::string> compatibleModules;
    Destruct::DObject*  compatibleModulesVector = dataType->getValue("compatibleModules").get<Destruct::DObject*>();
    DUInt64 modules = compatibleModulesVector->call("size").get<DUInt64>();

    for (DUInt64 index = 0; index < modules; index++)
      compatibleModules.push_back(compatibleModulesVector->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DUnicodeString>());

    std::string dataTypeName = dataType->getValue("name").get<Destruct::DUnicodeString>();
    std::string handlerName = dataType->getValue("handler").get<Destruct::DUnicodeString>();
    DataTypeHandler* handler = dataTypeManager->handler(handlerName);
    if (handler == NULL)
    {
       dataType->destroy();
       compatibleModulesVector->destroy();
       return (false);
    }
    //XXX XXX XXX compatible modules seem to not be copied must check it ! ?
    this->__dataTypes[dataTypeName] = new DataType(dataTypeManager->handler(handlerName), dataTypeName, compatibleModules);
    compatibleModulesVector->destroy();
    dataType->destroy();
  }
  vector->destroy();
 
  return (true);
}                       

Destruct::RealValue<Destruct::DObject*>      DataTypes::save(void) const //return Destruct::RealValue //ref count ?
{
  std::map<const std::string, const DataType* >::const_iterator type = this->__dataTypes.begin();
  Destruct::DObject*  dvector = Destruct::DStructs::instance().generate("DVectorObject");

  for (; type != this->__dataTypes.end() ; type++)
  {
    Destruct::DObject* dt = type->second->save(); 
    dvector->call("push", Destruct::RealValue<Destruct::DObject*>(dt)); 
    //dt->destroy();
  }
  return (dvector);
}

/**
 *  NodesTypes
 */
NodesTypes::NodesTypes()
{
}

const std::vector<const DataType* > NodesTypes::find(Node* node) const
{
  std::vector<const DataType* >     types;
  std::map<Node*, std::vector<const DataType*> >::const_iterator nodeDataTypes = this->__nodesDataTypes.find(node);
  if (nodeDataTypes != this->__nodesDataTypes.end())
    return (nodeDataTypes->second);

  return (types);
}

void    NodesTypes::insert(Node* node, const DataType* type)
{
  this->__nodesDataTypes[node].push_back(type);
}


bool                                         NodesTypes::load(Destruct::DValue const&  dobject)
{
  //XXX code me 
  return (false);
}                       

Destruct::RealValue<Destruct::DObject*>      NodesTypes::save(void) const //return Destruct::RealValue //ref count ?
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  Destruct::DStruct* dnodeStruct = destruct.find("DNode");
  Destruct::DStruct* dnodeTypesStruct = destruct.find("NodeTypes");
  Destruct::DStruct* dvectorStringStruct = destruct.find("DVectorString");
  Destruct::DObject* dvectorObject = destruct.generate("DVectorObject");

  std::map<Node*, std::vector<const DataType*> >::const_iterator nodeDataTypes = this->__nodesDataTypes.begin();
  for (; nodeDataTypes != this->__nodesDataTypes.end(); nodeDataTypes++) 
  {
    Destruct::DObject* dnodeTypes = dnodeTypesStruct->newObject();
    Destruct::DObject* dnode = dnodeStruct->newObject(); 
    
    dnode->setValue("absolute", Destruct::RealValue<Destruct::DUnicodeString>(nodeDataTypes->first->absolute()));
    dnodeTypes->setValue("node", Destruct::RealValue<Destruct::DObject*>(dnode)); //? intret ? y a rien de set
    //dnode->destroy();
    
    Destruct::DObject* dvectorString = dvectorStringStruct->newObject(); 
    std::vector<const DataType*>::const_iterator dataType = nodeDataTypes->second.begin();
    for (; dataType != nodeDataTypes->second.end(); dataType++)
      dvectorString->call("push", Destruct::RealValue<Destruct::DUnicodeString>((*dataType)->name()));
    dnodeTypes->setValue("dataTypes", Destruct::RealValue<Destruct::DObject*>(dvectorString));
    //dvectorString->destroy();
    dvectorObject->call("push", Destruct::RealValue<Destruct::DObject*>(dnodeTypes));
    //dnodeTypes->destroy();
  }
  
  return (dvectorObject); 
}

/**
 *  Return a DVectorString with type if exist or DNone 
 */
Destruct::RealValue<Destruct::DObject*> DataTypeManager::nodeDataType(Node* node)
{
  std::vector<const DataType*>  dataTypes = this->__nodesDataTypes.find(node); 
  if (dataTypes.size() == 0)
  {
    return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone);
  }

  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  Destruct::DStruct* dvectorStringStruct = destruct.find("DVectorString");
  Destruct::DObject* dvectorString = dvectorStringStruct->newObject(); 

  std::vector<const DataType*>::const_iterator dataType = dataTypes.begin();
  for (; dataType != dataTypes.end(); dataType++)
    dvectorString->call("push", Destruct::RealValue<Destruct::DUnicodeString>((*dataType)->name()));

  return (Destruct::RealValue<Destruct::DObject*>(dvectorString));
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
  this->__handlers.push_back(handler);
  return true;
}

DataTypeHandler* DataTypeManager::handler(const std::string& name) const
{
  std::list<DataTypeHandler*>::const_iterator handler = this->__handlers.begin();
  for (; handler != this->__handlers.end(); handler++)
  {     
     if ((*handler)->name == name)
        return (*handler);
  }
  return (NULL);
}

/**
 *  Search for Node type and return it as Variant Attributes
 */
Attributes	DataTypeManager::type(Node* node)
{
  Attributes					attributes;
  const std::vector<const DataType*> types = this->__dataType(node);
  
  if (types.size())
  {
    std::vector<const DataType*>::const_iterator type = types.begin();
    for (; type != types.end(); ++type)
    { 
      if (*type) //should never be null but .. 
        attributes[(*type)->handlerName()] = new Variant((*type)->name());
    }
  }
  return (attributes);
}

/**
 *  Search for node type and return it 
 *  If node found detect it, store it and return it
 */
const std::vector<const DataType*>  DataTypeManager::__dataType(Node* node)
{
  std::vector<const DataType*> types; 
  std::list<DataTypeHandler* >::iterator	handler;
  
  if (node == NULL)
    return (types);

  mutex_lock(&this->__mutex);
  types = this->__nodesDataTypes.find(node);
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
        std::cout << "Error : Can't get data type on node"  << std::endl;
        result = "error";
      }
      mutex_lock(&this->__mutex);
      const DataType* type = this->__dataTypes.find(result); //find for handler and insert if not coun
      if (type == NULL)
        type = this->__dataTypes.insert(*handler, result);
      this->__nodesDataTypes.insert(node, type);
      mutex_unlock(&this->__mutex);
    } 
  }
  mutex_lock(&this->__mutex);
  types = this->__nodesDataTypes.find(node);
  mutex_unlock(&this->__mutex);
  return (types);
}

/**
 *  Return list of compatible modules for Node passed as argument
 */
std::list<std::string>  DataTypeManager::compatibleModules(Node* node)
{
  std::list<std::string>   modules;
  Attributes attributes = node->dataType(); //node dataType could be overloaded so must call it
  Attributes::iterator attribute = attributes.begin(); 

  for (; attribute != attributes.end(); ++attribute)
  {
    std::string typeName = attribute->second->value<std::string>();
    mutex_lock(&this->__mutex);
    const DataType* type = this->__dataTypes.find(typeName);
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

void                    DataTypeManager::declare(void)
{
  Destruct::DType::init(); //XXX for test only must be done elswhere

  Destruct::DStructs&    destruct = Destruct::DStructs::instance();

  Destruct::DStruct* dtype = new Destruct::DStruct(NULL, "DataType", Destruct::DSimpleObject::newObject); 
  dtype->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "handler"));
  dtype->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"));
  dtype->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "compatibleModules"));
  destruct.registerDStruct(dtype);
}

void                    DataTypeManager::loadNodeDataTypes(Node* node, Destruct::DValue const& value)
{
  Destruct::DObject* dataTypesVector = value.get<Destruct::DObject*>();
  if (dataTypesVector == Destruct::DNone)
    return;

  DUInt64 dataTypesCount = dataTypesVector->call("size").get<DUInt64>();    
  for (DUInt64 currentIndex = 0; currentIndex < dataTypesCount; currentIndex++)
  { 
    Destruct::DUnicodeString dataTypeName = dataTypesVector->call("get", Destruct::RealValue<DUInt64>(currentIndex)).get<Destruct::DUnicodeString>();
    const DataType* dataType = this->__dataTypes.find(dataTypeName);
    if (dataType == NULL)
    {   
      std::cout << "Can't find datatype " << dataTypeName << std::endl;
      continue;
    }
    this->__nodesDataTypes.insert(node, dataType);
  }
  dataTypesVector->destroy();
}

bool                    DataTypeManager::load(Destruct::DValue value)
{
  try 
  {
    this->__dataTypes.load(value);
  }
  catch (Destruct::DException const& exception)
  {
  }

  return (true);
}

Destruct::DValue        DataTypeManager::save(void) const
{
  return (this->__dataTypes.save());
}
