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

#include "fso.hpp"
#include "node.hpp"
#include "vfs.hpp"
#include "rootnode.hpp"
#include "vlink.hpp"

//#include "dobject.hpp"
//#include "dvalue.hpp"
//#include "drealvalue.hpp"
//#include "dnullobject.hpp"
#include "dstructs.hpp"
#include "dsimpleobject.hpp"
#include "datatype.hpp"

//#include "session.hpp"
#include "log.hpp"

namespace DFF
{
/**
 *  Return singleton instance of VFS
 */
VFS&	VFS::Get()
{ 
  static VFS single;
  return single;
}

void    VFS::__declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  Destruct::DStruct*  dnodeStruct = new Destruct::DStruct(0, "DNode", Destruct::DSimpleObject::newObject); // rename NodeAttribute (on l'apply apres mais ca cree pas vraiment une node 
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUInt64Type, "uid"));
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "absolute")); //? 
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name")); 
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "tags")); 
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "children")); 
  dnodeStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "dataType"));
  destruct.registerDStruct(dnodeStruct);

  destruct.registerDStruct(Destruct::makeNewDCpp<NodeContainer>("NodeContainer"));

//XXX doublon avec ce ux method load pour ntfs 
  Destruct::DStruct*  vlinkStruct = new Destruct::DStruct(0, "VLink", Destruct::DSimpleObject::newObject);
  vlinkStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "linkedNode"));
  vlinkStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"));
  destruct.registerDStruct(vlinkStruct);

  Destruct::DStruct*  dpathStruct = new Destruct::DStruct(0, "DPath", Destruct::DSimpleObject::newObject);
  dpathStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "path")); 
  destruct.registerDStruct(dpathStruct);

  destruct.registerDStruct(Destruct::makeNewDCpp<VoidNode>("VoidNode"));

  Destruct::DStruct* dvlinkStruct = new Destruct::DStruct(0, "DVLink", Destruct::DSimpleObject::newObject);
  dvlinkStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"));
  dvlinkStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "node"));
  dvlinkStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "linkNode"));
  destruct.registerDStruct(dvlinkStruct);

  destruct.registerDStruct(Destruct::makeNewDCppSingleton<Time>("Time"));
  destruct.registerDStruct(Destruct::makeNewDCppSingleton<Log>("Log"));

  //XXX c lui qui devrait load c celui qu igere les ession enfin bon ..


  //SessionLoader::declare(); XXX load by destruct
}


static Destruct::DStruct vfsStruct = Destruct::DStruct(NULL, "VFS", VFS::newObject, VFS::ownAttributeBegin(), VFS::ownAttributeEnd()); 

/**
 *  Construct VFS and register root node '/'
 */
VFS::VFS() : DCppObject<VFS>(&vfsStruct), __nodeManager(*this) //XXX
{
  this->__declare();
  this->root = new VFSRootNode("/");
  this->registerNode(this->root);
  cwd = root;
  this->init();
}

VFS::~VFS()
{
}

DObject* VFS::clone() const //special for singleton
{
        ////return (new CppClass(*static_cast<const CppClass *>(this)));
  return (const_cast<VFS *>(this));
}

/**
 *  Handle Event
 */
void    VFS::Event(event *e)
{
}

/**
 *  Change current working directory
 */
void    VFS::cd(Node *path)
{
  cwd = path;
}

/**
 *  Return current working directory
 */
Node*   VFS::GetCWD(void)
{
  return (cwd);
}

/**
 *  Register FSobj to a list and return an UID 
 */
uint16_t	VFS::registerFsobj(fso* fsobj) throw (vfsError)
{
  if (fsobj != NULL)
    this->__fsobjs.push_back(fsobj);
  else
    throw (vfsError("registerFsobj() NULL pointer provided"));
  return (uint16_t)(this->__fsobjs.size());
}

/**
 *  Return an ID and register ndoe in the node/ID manager
 */
uint64_t        VFS::registerNode(Node* node)
{
  return (this->__nodeManager.uid(node));
}

/**
 *  Deleted Node and remove them from the UID manager recursively
 */
void    VFS::__deleteNode(Node* node)
{
  std::vector<Node*> children = node->children();
  std::vector<Node*>::const_iterator child = children.begin();
  for (; child != children.end(); ++child)
    this->__deleteNode(*child);
  Node* parent = node->parent();
  if (parent)
    parent->removeChild(node);
  this->__nodeManager.remove(node);
  delete node;
}

/** 
 *  Unregister node from the tree by removing it in parent list
 *  Use delete node recursively to delete and unregister the tree
 *  Node destructor call this method (must be used on root/registered root)
 *  Event is sent before deletion so registered event can clear data recursively before deletion
 */
bool            VFS::unregister(Node* node) 
{
  if (node == NULL)
    return (false);

  VLink*  vlink = dynamic_cast<VLink*>(node);
  fso* fsobj = node->fsobj();
  if (vlink || (fsobj && fsobj->name == "Bookmarks"))
  {
    event* e = new event;
    e->type = 0x0de1; 
    e->value = Variant_p(new Variant(node));
    this->notify(e);

    this->__deleteNode(node);
    return (true);
  }
  else 
  {
    //Not implemented yet
    throw ("Not implemented");
    //node->parent()->removeChild(node); //XXX if we want do handle it ... 
    //fsobj->unamp(node);
    //fsobj->remove(node);
    return (true);
  }

  return (false);
}

/**
 *  Return lists of all registered fsobjs
 */
std::vector<fso*>	VFS::fsobjs()
{
  return (this->__fsobjs);
}

/**
 *  Return current total of nodes registered in the VFS and fsobjs
 */
uint64_t	VFS::totalNodes(void)
{
  size_t	i;
  uint64_t	totalnodes;

  totalnodes = this->__nodeManager.orphansCount();
  for (i = 0; i != this->__fsobjs.size(); i++)
    totalnodes += this->__fsobjs[i]->nodeCount();
  return (totalnodes);
}

/**
 *  Return node by path from starting point Node where
 */
Node* VFS::GetNode(std::string path, Node* where)
{
  std::vector<Node *>	next;
  uint32_t		i;

  if (path == "..")
    return (where->parent());
  if (where->hasChildren())
  {
      next = where->children();
      for (i = 0; i < next.size(); i++)
      {
	if (next[i]->name() == path)
	  return (next[i]); 
      }
      return (0);
  }
  else
    return (0);
}

/**
 *  Return node by path
 */
Node* VFS::GetNode(std::string path)
{
  if (path == "/")
    return root;	
  path = path.substr(path.find('/') + 1);
  std::string lpath;
  std::string rpath = path;
  Node* tmp = root;
  do
  {
    if (rpath.find('/') != std::string::npos)
    {
      lpath = rpath.substr(0, rpath.find('/'));
      rpath = rpath.substr(rpath.find('/') + 1); 
    }
    else
    { 
      lpath = rpath;
      rpath = "";
    }
    tmp = GetNode(lpath, tmp);
  }  while (tmp && rpath.size());
  return (tmp);
}

Destruct::DValue VFS::getNode(Destruct::DValue args) //swig didn't support const&
{
  return (RealValue<DObject* >(this->GetNode(args.get<DUnicodeString>()))); 
}

/**
 *  Return node by id
 */
Node*	VFS::getNodeById(uint64_t id)
{
  uint16_t fsoid = id >> 48;

  if (fsoid == 0)
    return (this->__nodeManager.node(id)); //orphans / unlinked
  else if ((fsoid > 0) && ((fsoid - 1) < (uint16_t)this->__fsobjs.size()))
  {
    fso* fsobj = this->__fsobjs[fsoid - 1];
    if (fsobj)
      return fsobj->getNodeById(id);
  }
  return (NULL);
}

NodeManager&    VFS::nodeManager(void) 
{
  return (this->__nodeManager);
}

void VFS::addDNodeID(uint64_t oldId, uint64_t newId)
{
  this->__dnodeId[oldId] = newId;
}

Node* VFS::getNodeByDUid(uint64_t oldid)
{
  uint64_t newId = this->__dnodeId[oldid];
  return this->getNodeById(newId); 
}


/**
 *  Node Manager
 *  This class manager node by giving them an unique id 
 *  that permet to track node and get them back from that uid
 */
NodeManager::NodeManager(VFS& vfs) : __nextId(0), __vfs(vfs), __dataTypeManager(DataTypeManager::Get())
{
}

/**
 *  Register a node by Id or return the node id
 */
uint64_t NodeManager::uid(Node* node)
{
  uint64_t uid = 0;
  fso*  fsobj = node->fsobj();
  VLink* vlink = dynamic_cast<VLink*>(node); 

  //if this->__saveNodeBase
  //{
    //search node Id in base 

    //remove uid from base (pour pas que ca soit trop gros)
    
    //return (uid);
  //}

  if (fsobj && !vlink)
  {
    uid = (uint64_t)fsobj->uid() << 48;
   // if (node->parent()) //XXX for NTFS and module that deleted node,  what happen if parent is linked after ?
   // {
      uid |= this->__nextId;
      node->fsobj()->registerNode(uid, node);
      this->__nextId++;
      return (uid);
   // }
  }
  uid |= __nextId;
  this->__orphans[uid] = node; //XXX save too
  this->__nextId++;
  return (uid);
}

/** 
 *  Return node not managed by fso from an uid (fso orphans)  
 */ 
Node* NodeManager::node(uint64_t uid) const
{
  std::map<uint64_t, Node* >::const_iterator node = this->__orphans.find(uid);
  if (node != this->__orphans.end())
    return (node->second);
  return (NULL);
}

/**
 *  Return number of nodes not managed by fso (fso orphans)
 */
uint64_t	NodeManager::orphansCount() const
{
  return (this->__orphans.size());
}

/**
 *  Remove node by uid from fso orphans nodes list
 */
bool  NodeManager::remove(uint64_t uid)
{
  if (this->__orphans.erase(uid) == 0)
    return (false);
  return (true);
}

/**
 *  Remode node from fso orphans nodes list
 */
bool  NodeManager::remove(Node* node)
{
  return (this->remove(node->uid())); 
}

bool  NodeManager::load(Destruct::DValue const& data)
{
  //Destruct::DObject* dnode = data.get<Destruct::DObject*>();

  std::cout << "creating children tree" << std::endl;
  //NodeId* rootId = new NodeId(dnode);
  //std::cout << "child tree is created " << std::endl;
 
  /*

  Destruct::DObject* children = dnode->getValue("children").get<Destruct::DObject*>();

  std::map<const std::string, Node*> nodeMap;
  std::vector<Node* > nodes = this->__vfs.GetNode("/")->children();
  std::vector<Node* >::const_iterator cnode = nodes.begin(); 
  for (; cnode != nodes.end(); cnode++)
    nodeMap[(*cnode)->name()] = (*cnode);

  DUInt64 count = children->call("size").get<DUInt64>();
  for (DUInt64 index = 0; index < count; index++)
  {
    Destruct::DObject* value = children->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DObject*>();
    std::string nodeName = value->getValue("name").get<Destruct::DUnicodeString>(); 
    if (nodeName != "Bookmarks") //now in /case/bookmarks but with bookmarks->fso == bookmarks
      this->__loadNode(value, nodeMap[nodeName]);
    value->destroy();
    value->destroy();
  }
  children->destroy();

  std::cout << dnode->refCount() << std::endl;
  dnode->destroy(); */
  return true;
}

void    NodeManager::__loadNode(Destruct::RealValue<Destruct::DObject*> dobject, Node* node)//Node* node 
{
  Destruct::DObject* dnode = dobject;


  //push dataType of node in node dataTypes
  this->__dataTypeManager->loadNodesType(node, dnode->getValue("type"));


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
    nodeMap[(*cnode)->name()] = (*cnode);

  Destruct::DObject* children = dnode->getValue("children").get<Destruct::DObject*>();
  DUInt64 count = children->call("size").get<DUInt64>();

  for (DUInt64 index = 0; index < count; index++)
  {
    Destruct::DObject* childObject = children->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DObject*>();
    std::string name = childObject->getValue("name").get<Destruct::DUnicodeString>();
    Node* childNode = nodeMap[name];
    if (childNode == NULL)
    {
      std::cout << "can't find node " << name << std::endl;
      continue;
    }
    this->__loadNode(Destruct::RealValue<Destruct::DObject*>(childObject), childNode);
    childObject->destroy();
    childObject->destroy(); 
  }
  children->destroy();
}


Destruct::DValue  NodeManager::save(void) const
{
  return Destruct::RealValue<Destruct::DObject* >(Destruct::DNone);
}


//void    NodeManager::setId(Destruct::RealValue<Destruct::DObject*> dobject)
//{
  //Destruct::DObject* dnode = dobject;
  //Destruct::DObject* children = dnode->getValue("children").get<Destruct::DObject*>();
  //DUInt64 count = children->call("size").get<DUInt64>(); 
  //for (DUInt64 index = 0; index < count; index++)
    //this->setId(children->call("get", Destruct::RealValue<DUInt64>(index))); 
//}

///**
 //*  Load previous id database
 //*/
//void    NodeManager::load(Destruct::DValue const& base)
//{
  //Destruct::DObject* root = base.get<Destruct::DObject*>();
  //this->setId(root);
//}

/** 
 * NodeContainer XXX temp
 */
NodeContainer::NodeContainer(Destruct::DStruct* dstruct, Destruct::DValue const& args) : DCppObject(dstruct, args), __node(NULL)
{
  this->init();
}

NodeContainer::NodeContainer(Destruct::DStruct* dstruct, Node* node) : DCppObject(dstruct), absolute(node->absolute()), __node(node)
{
  this->init();
}

NodeContainer::~NodeContainer()
{
}

Node*   NodeContainer::node(void)
{
  if (this->__node == NULL)
  {
    VFS& vfs = VFS::Get();
    this->__node = vfs.GetNode(this->absolute); //XXX ou tghis-absloute9)
  }
  return (this->__node);
}

}
