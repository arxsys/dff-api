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

#include "rootnode.hpp"
#include "vfs.hpp"
#include "vlink.hpp"
#include "fso.hpp"

VFSRootNode::VFSRootNode(std::string name) : Node()//: Node(name) register manually to avoid recursive lock
{
  this->__name = name;
  this->setParent(this);
  this->setDir();
}

VFSRootNode::~VFSRootNode()
{
}

ModulesRootNode::ModulesRootNode(EventHandler* vfs, Node* root) : Node(std::string("Modules root"))
{
  mutex_init(&this->__mutex);
  this->setParent(root);
  root->addChild(this);
  vfs->connection(this);
}

ModulesRootNode::~ModulesRootNode()
{
  mutex_destroy(&this->__mutex);
}

std::string ModulesRootNode::icon()
{
  return std::string(":module2.png");
}

void ModulesRootNode::Event(event* e)
{
  RCPtr<Variant>     variantNode = e->value;
 
  if (variantNode)
  {
    Node* root = variantNode->value<Node *>();
    if (root && (!dynamic_cast<VLink*>(root)) && root->parent()->parent()->absolute() != "/")
    {
      if (root->fsobj())
      {
        Node* moduleRoot;

        mutex_lock(&this->__mutex);
        std::map<std::string, Node*>::iterator  it = this->__modulesNameRootNode.find(root->fsobj()->name);
        if (it == this->__modulesNameRootNode.end()) 
        {
          moduleRoot = new Node(root->fsobj()->name, 0, this);
          this->__modulesNameRootNode[root->fsobj()->name] = moduleRoot;
        }
        else
          moduleRoot = it->second;
        
        std::vector<Node* >  children = moduleRoot->children();
        std::vector<Node* >::iterator   child = children.begin();
        for (; child != children.end(); child++)
        {
           VLink* childLink = NULL;
           if (((childLink = dynamic_cast<VLink* >(*child)) != NULL) && (childLink->linkNode() == root->parent()))
           {
              mutex_unlock(&this->__mutex);
              return ; 
           }
        }
        VLink* link = new VLink(root->parent(), moduleRoot);
        event*  e = new event;
        e->value = Variant_p(new Variant(link));
        mutex_unlock(&this->__mutex);
        VFS::Get().notify(e);
      }
    }
  }
}

/**
 *  IconNode : node which take an icon name as parameter 
 */
IconNode::IconNode(Node* parent, const std::string& name, const std::string& iconName, const std::string& fsoName): Node(name, 0, parent, new SimpleFso(fsoName)), __iconName(iconName)
{
}

std::string     IconNode::icon(void)
{
  return (this->__iconName);
}

/**
 *  SimpleFso
 */

SimpleFso::SimpleFso(const std::string& name) : fso(name)
{
}

void      SimpleFso::start(std::map<std::string, RCPtr< Variant > > args)
{
}

int32_t   SimpleFso::vopen(class Node *n)
{
  return (-1);
}

int32_t   SimpleFso::vread(int32_t fd, void *rbuff, uint32_t size)
{
  return (-1);
}

int32_t  SimpleFso::vwrite(int32_t fd, void *wbuff, uint32_t size)
{
  return (-1);
}

int32_t  SimpleFso::vclose(int32_t fd)
{
  return (-1);
}

uint64_t SimpleFso::vseek(int32_t fd, uint64_t offset, int32_t whence)
{
  return ((uint64_t)-1);
}

uint32_t  SimpleFso::status(void)
{
  return (0);
};

uint64_t SimpleFso::vtell(int32_t fd)
{
  return (0);
}
