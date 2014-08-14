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

//#include "../destruct/drealvalue.hpp" //fix CMake include directories

//namespace Destruct
//{
  //class DValue;
  //class DObject;
  ////class RealValue;
//};

class Node;
class fso;
class vfsError;
class VfsNode;

class NodeManager
{
public:
                                NodeManager(void);
  uint64_t                      uid(Node* node);
  bool                          remove(uint64_t uid);
  bool                          remove(Node* node);
  Node*                         node(uint64_t uid) const;
  uint64_t                      orphansCount(void) const;
  
  //void                          load(Destruct::DValue const& base);
  //void                          setId(Destruct::RealValue<Destruct::DObject*> dnode);
private:
  //std::map<uint64_t, RealValue<Destruct::DObject*> > __saveBase;
  std::map<uint64_t, Node* >   __orphans;
  uint64_t                     __nextId;
};

class VFS : public EventHandler
{  
private:
  EXPORT 	                VFS();
  EXPORT                        ~VFS();
  VFS&                          operator=(VFS&);
                                VFS(const VFS&);
  void                          __deleteNode(Node* node);
  std::vector<fso*>	        __fsobjs;
  NodeManager                   __nodeManager;

  //std::map<uint64_t, Destruct::DObject* > __tmpTree; //SessioNTree ? 
public:
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

  //EXPORT void                   setId(Destruct::RealValue<Destruct::DObject*> dnode);
  //EXPORT Destruct::DObject*     toDNode(Node* node) const;
  //EXPORT void                   load(Destruct::DValue dobject); //const ref ..
  //EXPORT Destruct::DValue       save(void) const;
};

#endif
