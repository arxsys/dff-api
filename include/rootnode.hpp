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
 *  Solal Jacob <sja@digital-forensic.org>
 */

#ifndef __ROOTNODE_HPP__
#define __ROOTNODE_HPP__

#include <string>
#include <set>
#include "eventhandler.hpp"
#include "threading.hpp"
#include "node.hpp"
#include "fso.hpp"

class VFSRootNode : public Node
{
public:
  EXPORT VFSRootNode(std::string name);
  EXPORT ~VFSRootNode();
};

class ModulesRootNode: public Node, public EventHandler 
{
private:
                               mutex_def(__mutex);
  std::map<std::string, Node*> __modulesNameRootNode;
public:
  EXPORT                ModulesRootNode(EventHandler* vfs, Node* root); //EventHandler*, DObject* root .... pour python 
  EXPORT                ~ModulesRootNode();
  EXPORT std::string    icon();
  EXPORT void           Event(event* e); 
};

class IconNode : public Node
{
public:
  EXPORT                IconNode(Node* parent, const std::string& name, const std::string& iconName, const std::string& fsoName); //Node* a remplacer par Dobject* 
  EXPORT std::string    icon(void);
private:
  const std::string     __iconName;
};

class SimpleFso : public fso
{
public:
  SimpleFso(const std::string& name);

  void      start(std::map<std::string, RCPtr< Variant > > args);
  int32_t   vopen(class Node *n);
  int32_t   vread(int32_t fd, void *rbuff, uint32_t size);
  int32_t   vwrite(int32_t fd, void *wbuff, uint32_t size);
  int32_t   vclose(int32_t fd);
  uint64_t  vseek(int32_t fd, uint64_t offset, int32_t whence);
  uint32_t  status(void);
  uint64_t  vtell(int32_t fd); 
};

#endif
