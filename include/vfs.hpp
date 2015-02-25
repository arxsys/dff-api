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

// \brief This class implements the virtual file system of the framework.

// The Virtual File System (VFS) is a central point of the framework.
// It permits to register nodes and browse them.

#ifndef __VFS_HPP__
#define __VFS_HPP__

#ifndef WIN32
  #include <stdint.h>
#elif _MSC_VER >= 1600
  #include <stdint.h>
#else
  #include "wstdint.h"
#endif
#include <vector>
#include <deque>
#include <list>
#include <set>

#include "eventhandler.hpp"
#include "export.hpp"
#include "exceptions.hpp"

#include "dstructs.hpp"
#include "dvalue.hpp"
#include "protocol/dcppobject.hpp"

class Node;
class fso;
class vfsError;
class VfsNode;
class VFS;
class DataTypeManager;

class NodeManager
{
public:
                                NodeManager(VFS& vfs);
  uint64_t                      uid(Node* node);
  bool                          remove(uint64_t uid);
  bool                          remove(Node* node);
  Node*                         node(uint64_t uid) const;
  uint64_t                      orphansCount(void) const;
 
                
  bool                          load(Destruct::DValue const& base); //load base before loading module to return them the id
                                                                   //ntfs node name is set after uid  
//  void                          clean(void);  call at end of loading to destroy NodeId ?

  Destruct::DValue              save(void) const; //return a base of uid and node 
   
 
  //void                          load(Destruct::DValue const& base);
  //void                          setId(Destruct::RealValue<Destruct::DObject*> dnode);
private:
  //std::map<uint64_t, RealValue<Destruct::DObject*> > __saveBase;

  std::map<uint64_t, Node* >   __orphans;
  uint64_t                     __nextId;
  VFS&                         __vfs;
  DataTypeManager*             __dataTypeManager;
  void                         __loadNode(Destruct::RealValue<Destruct::DObject*> dobject, Node* node);//Node* node 
};

class VFS : public EventHandler, public Destruct::DCppObject<VFS>
{  
private:
  EXPORT 	                VFS();
  EXPORT                        ~VFS();

  VFS&                          operator=(VFS&);
                                VFS(const VFS&);


  void                          __deleteNode(Node* node);
  std::vector<fso*>	        __fsobjs;
  NodeManager                   __nodeManager;

  void                          __declare(void);
  //std::map<uint64_t, Destruct::DObject* > __tmpTree; //SessioNTree ? 
 
  std::map<uint64_t, uint64_t> __dnodeId; //xxx temp test 
public:
  virtual DObject*                      clone(void) const;     


  static Destruct::DObject* newObject(Destruct::DStruct * dstruct, Destruct::DValue const& args)
  {
    return (&VFS::Get());
  }

  void  addDNodeID(uint64_t duid, uint64_t uid); //xxx temp
  
  Node* getNodeByDUid(uint64_t oldid);

  class Node*                   cwd;
  Node*		                root;
  EXPORT static VFS&            Get();
  EXPORT uint64_t               registerNode(Node* n);
  EXPORT uint16_t	        registerFsobj(fso* fsobj) throw (vfsError);
  EXPORT bool                   unregister(Node* node); 
  EXPORT virtual void	        Event(event *e);
  EXPORT void 	                cd(Node *);
  EXPORT Node* 	                GetCWD(void);
  EXPORT Node*	                GetNode(std::string path);
  EXPORT Node*	                GetNode(std::string path, Node* where);
  EXPORT std::vector<fso*>	fsobjs();
  EXPORT uint64_t	        totalNodes();
  EXPORT Node*		        getNodeById(uint64_t id);

  EXPORT NodeManager&           nodeManager(void);
  //EXPORT void                   setId(Destruct::RealValue<Destruct::DObject*> dnode);
  //EXPORT Destruct::DObject*     toDNode(Node* node) const;
  //EXPORT void                   load(Destruct::DValue dobject); //const ref ..
  //EXPORT Destruct::DValue       save(void) const;

  Destruct::DValue                        getNode(Destruct::DValue args); //for swig
/**
 *  Destruct declaration
 */
public:
  Destruct::RealValue<Destruct::DFunctionObject* >        _getNode;        

  static size_t ownAttributeCount()
  {
    return (0);
  }

  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
       Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "getNode", Destruct::DType::DUnicodeStringType),
    };
    return (attributes);
  }

  static Destruct::DPointer<VFS>* memberBegin()
  {
    static Destruct::DPointer<VFS> memberPointer[] = 
    {
      Destruct::DPointer<VFS>(&VFS::_getNode, &VFS::getNode),
      //DPointer<VoidNode>(&VoidNode::children),
    };
    return (memberPointer);
  }

  static Destruct::DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static Destruct::DPointer<VFS>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

namespace Destruct{
template<>
inline DObject* DCppObject<VFS>::clone() const
{
  return (const_cast<DCppObject<VFS> *>(this));
}

}
class NodeContainer : public Destruct::DCppObject<NodeContainer>
{
public:
                                                   NodeContainer(Destruct::DStruct* dstruct, Destruct::DValue const& args);
                                                   NodeContainer(Destruct::DStruct* dstruct, Node* node);
                                                   ~NodeContainer();
  Node*                                            node(void);
  Destruct::RealValue<Destruct::DUnicodeString>    absolute; 

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (1);
  }

  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
      Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "absolute"), //XXX ? 
      
    };
    return (attributes);
  }

  static Destruct::DPointer<NodeContainer>* memberBegin()
  {
    static Destruct::DPointer<NodeContainer> memberPointer[] = 
    {
      Destruct::DPointer<NodeContainer>(&NodeContainer::absolute),
    };
    return (memberPointer);
  }

  static Destruct::DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static Destruct::DPointer<NodeContainer >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
private:
  Node*         __node;
};


#endif
