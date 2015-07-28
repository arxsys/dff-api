#include "session.hpp"
#include "dstructs.hpp"
#include "dsimpleobject.hpp"
#include "protocol/dstream.hpp"
//#include "protocol/dserialize.hpp"
#include "protocol/dmutableobject.hpp"
#include "dattribute.hpp"
#include "dstruct.hpp"

#include "datatype.hpp"
#include "vfs.hpp"
#include "node.hpp"
#include "tags.hpp"
#include "vlink.hpp"

#include <list>

#include "configuration.hpp"

/**
 *  SessionLoader (temp for DLoader/DSerializer)
 */
SessionLoader::SessionLoader(DStruct* dstruct, DValue const& args) : DCppObject(dstruct, args) , __destruct(Destruct::DStructs::instance())
{
  this->init();
}

SessionLoader::~SessionLoader()
{
}

void            SessionLoader::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  Destruct::DStruct* configuration = new Destruct::DMutableStruct(NULL, "Configuration", Configuration::newObject, Configuration::ownAttributeBegin(), Configuration::ownAttributeEnd());
  destruct.registerDStruct(configuration);

  Destruct::DStruct* caseInformations = new Destruct::DMutableStruct(NULL, "CaseInformations", CaseInformations::newObject, CaseInformations::ownAttributeBegin(), CaseInformations::ownAttributeEnd());
  destruct.registerDStruct(caseInformations);

  Destruct::DStruct* session = new Destruct::DStruct(NULL, "DFS", DFS::newObject, DFS::ownAttributeBegin(), DFS::ownAttributeEnd());
  destruct.registerDStruct(session);

  Destruct::DStruct*  argumentStruct = new Destruct::DStruct(0, "ModuleArguments", Destruct::DSimpleObject::newObject);
  argumentStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "moduleName"));
  argumentStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "arguments"));
  argumentStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "save"));
  destruct.registerDStruct(argumentStruct);

  Destruct::DStruct* sessionLoader = Destruct::makeNewDCpp<SessionLoader>("SessionLoader");
  destruct.registerDStruct(sessionLoader);
}


Destruct::DObject* SessionLoader::load(Destruct::DValue filePath)
{
  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Input));
  
  try 
  {
    Destruct::DObject* dstream = this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg));
    arg->destroy();

    Destruct::DObject* deserializer = this->__destruct.generate("DeserializeBinary", RealValue<DObject*>(dstream));
    Destruct::DObject* session = deserializer->call("DObject").get<DObject*>();
    deserializer->destroy();
    dstream->destroy();
    std::cout << "return loaded session ref " << session->refCount() << std::endl;

    return (session);
  } 
  catch (std::string const& error)
  {
    std::cout << "Error loading file '" << filePath.asUnicodeString() << "' : " << error << std::endl; 
  }
  catch (Destruct::DException const& exception)
  {
     std::cout << "Error loading file '" << filePath.asUnicodeString() << "' : " << exception.error() << std::endl;
  }
  catch (std::bad_alloc const& exception)
  {
    std::cout << "Error loading file '" << filePath.asUnicodeString() << "' : " << " Can't allocate object " << exception.what() << std::endl;
  }
  return (Destruct::DNone);
}

/** 
 * DFS
*/
DFS::DFS(Destruct::DStruct* dstruct, Destruct::DValue const& args) : Destruct::DCppObject<DFS>(dstruct, args), __vfs(VFS::Get()), __destruct(Destruct::DStructs::instance()), __dataTypeManager(DataTypeManager::Get())
{
  this->init();
  this->dstructs = Destruct::DStructs::instance().generate("DVectorStruct");
  this->modules = Destruct::DStructs::instance().generate("DVectorObject");
}

DFS::~DFS()
{
  std::cout << "~DFS()" << std::endl;
}

//load incremental
/*
  loadDStruct
  Object* nodeTree dserialize
  OBject* dataTree dserialize
  Object* modules dserialize
  Object* report dserialize
*/

void    DFS::save(Destruct::DValue const& filePath) 
{
  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Output));
  DObject* dstream = this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg));
  arg->destroy();
  //Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");
  Destruct::DObject* serializer = this->__destruct.generate("SerializeBinary", RealValue<DObject*>(dstream)); 


  //serialize each object rather than all object tree to use less memory ? (save incremental ?)
  this->__saveDStruct();
  this->nodeTree = this->__toDNodeTree(this->__vfs.root); 
  this->dataType = this->__dataTypeManager->save();
  this->tags = TagsManager::get().save();

  serializer->call("DObject", RealValue<DObject*>(this));
  serializer->destroy();
  dstream->destroy();
  //std::cout << "DFS::Save(" << filePath.asUnicodeString() << ") finish"  << std::endl;
}

void    DFS::__saveDStruct(void) const
{
  DObject* dstructs = this->dstructs;
  for (size_t index = 0; index != this->__destruct.count(); index++)
  {
    Destruct::DStruct* dstruct = this->__destruct.find(index);
    if (dstruct->name().find("ArgumentsTest") != std::string::npos)
    {
      dstructs->call("push", RealValue<DStruct*>(dstruct)); //if object is none it doesn't throw anything want call is called that;s strange  must return function didn't exist 
    }
  }
}

Destruct::DObject*    DFS::__toDNodeTree(Node* node) const
{
  if (node->path().find("/Case/Bookmarks") == 0)
  {
    VLink* vlink = dynamic_cast<VLink*>(node);
    if (vlink)
    {
      Destruct::DObject* dvlink = this->__destruct.generate("VLink");
      dvlink->setValue("name", Destruct::RealValue<Destruct::DUnicodeString>(vlink->name()));
      dvlink->setValue("linkedNode", Destruct::RealValue<Destruct::DUnicodeString>(vlink->linkNode()->absolute())); 
      return (dvlink);
    }
  }
  Destruct::DObject* dnode = this->__destruct.generate("DNode");
  Destruct::DObject* dchildren = this->__destruct.generate("DVectorObject"); 

  dnode->setValue("uid", Destruct::RealValue<DUInt64>(node->uid()));
  dnode->setValue("name", Destruct::RealValue<Destruct::DUnicodeString>(node->name()));
  
  std::vector<Tag_p> tags = node->tags();
  std::vector<Tag_p>::const_iterator tag = tags.begin();
  Destruct::DObject* tagList = this->__destruct.generate("DVectorString");
  for (; tag != tags.end(); tag++)
    tagList->call("push", Destruct::RealValue<Destruct::DUnicodeString>((*tag)->name()));

  dnode->setValue("tags", Destruct::RealValue<Destruct::DObject*>(tagList));
  tagList->destroy();
 
  Destruct::DObject*  nodeDataType = this->__dataTypeManager->nodeDataType(node); 
  dnode->setValue("dataTypes",  Destruct::RealValue<Destruct::DObject*>(nodeDataType)); //XXX //remplace By DataTypeId (pour la place) ??
  nodeDataType->destroy();  
 
  std::vector<Node *> children = node->children();
  std::vector<Node *>::const_iterator child = children.begin();
  for (; child != children.end(); child++)
  { 
    DObject* dchild = this->__toDNodeTree(*child);
    dchildren->call("push", Destruct::RealValue<Destruct::DObject*>(dchild));
    dchild->destroy();    
  }
  dnode->setValue("children", Destruct::RealValue<Destruct::DObject*>(dchildren));
  dchildren->destroy(); 
  return (dnode);
}

/**
 *   Loading related method
 */
void    DFS::loadTree(void)
{
  Destruct::DObject* dnode = this->nodeTree;
  Destruct::DObject* children = dnode->getValue("children").get<Destruct::DObject*>();

  std::map<const std::string, Node*> nodeMap;
  std::vector<Node* > nodes = this->__vfs.GetNode("/")->children();
  std::vector<Node* >::const_iterator cnode = nodes.begin(); 
  for (; cnode != nodes.end(); cnode++)
    nodeMap[(*cnode)->name()] = (*cnode);

  DUInt64 count = children->call("size").get<DUInt64>();
  for (DUInt64 index = 0; index < count; index++)
  {
    Destruct::DObject* value = children->call("get", Destruct::RealValue<DUInt64>(index)).get<DObject*>();
    Destruct::DUnicodeString nodeName = value->getValue("name").get<Destruct::DUnicodeString>(); 
  
    Node* node = nodeMap[nodeName];
    //if (node == NULL)
    //std::cout << "Load Tree node is NULL " << std::endl;
    //else
    if (node != NULL)
    {
       //std::cout << " dnode " << nodeName << " id " << value->getValue("uid").get<DUInt64>() << " " << node->name() << " id " << node->uid() << std::endl;
      this->__vfs.addDNodeID(value->getValue("uid").get<DUInt64>(), node->uid());
      if (nodeName != "Bookmarks")
        this->__loadNode(value, node);
    }
    value->destroy();
  }
  children->destroy();
}

void    DFS::__loadNode(Destruct::RealValue<Destruct::DObject*> dobject, Node* node)//Node* node 
{
  Destruct::DObject* dnode = dobject;

  this->__vfs.addDNodeID(dnode->getValue("uid").get<DUInt64>(), node->uid());
  //this->__vfs->nodeManager->loadTree()

  //push dataType of node in node dataTypes
  this->__dataTypeManager->loadNodeDataTypes(node, dnode->getValue("dataTypes"));


  //set Node tags
  Destruct::DObject* tagList = dnode->getValue("tags").get<Destruct::DObject*>();
  DUInt64 tagCount = tagList->call("size").get<DUInt64>();
  for (DUInt64 index = 0; index < tagCount; index++)
  {
    Destruct::DUnicodeString tagName = tagList->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DUnicodeString>();
    node->setTag(tagName);
  }
  tagList->destroy();

  //iterate
  std::map<const std::string, Node*> nodeMap;
  std::vector<Node* > nodes = node->children();
  std::vector<Node* >::const_iterator cnode = nodes.begin(); 
  for (; cnode != nodes.end(); cnode++)
  {
    //if (*cnode)->name() not already in nodeMap checker les doublons ...(en plus la c le dernier qui est mis ...)
    nodeMap[(*cnode)->name()] = (*cnode);
  }

  Destruct::DObject* children = dnode->getValue("children").get<Destruct::DObject*>();
  DUInt64 count = children->call("size").get<DUInt64>();

  for (DUInt64 index = 0; index < count; index++)
  {
    Destruct::DObject* childObject = children->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DObject*>();
    Destruct::DUnicodeString name = childObject->getValue("name").get<Destruct::DUnicodeString>();
    Node* childNode = nodeMap[name];
    if (childNode == NULL)
    {
      std::cout << "can't find node " << name << std::endl;
      continue;
    }
    this->__loadNode(Destruct::RealValue<Destruct::DObject*>(childObject), childNode);
    childObject->destroy();
  }
  children->destroy();
}

void Debug::inspect(Destruct::DObject* dobject)
{
  Destruct::DStruct* dstruct = dobject->instanceOf();

  if (dstruct->name() == "DVectorObject")
  {
    DUInt64 count = dobject->call("size").get<DUInt64>();
    for (DUInt64 index = 0; index < count; index++)
    {
      Destruct::DObject* subobject = dobject->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DObject*>();
      std::cout << "  contain : " << subobject->instanceOf()->name() << " refcount : " << subobject->refCount()  - 2 <<  std::endl;
      Debug::inspect(subobject);
      subobject->destroy();
    }
    return ; 
  }

  for (Destruct::DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    if ((*attribute).type().getType() == Destruct::DType::DObjectType) 
    {
       Destruct::DObject* object = dobject->getValue((*attribute).name()).get<Destruct::DObject*>();
       std::cout << "  " << dobject->instanceOf()->name() << "::" << (*attribute).name() << " type: " << object->instanceOf()->name() << " refCount: " << object->refCount() - 1<<  std::endl;
       Debug::inspect(object); 
       object->destroy();
    }   
  }
  std::cout << "Inspecting DVectorObject (refCount) : " << dobject->refCount() << std::endl;
}
