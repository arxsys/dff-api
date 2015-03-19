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

#include "variant.hpp"
#include "tags.hpp"
#include "vlink.hpp"
#include "vfs.hpp"

VLink::VLink(Node* linkedNode, Node* parent, std::string newname) : Node()
{
  if (!parent)
    throw std::string("Can't create VLink to a NULL parent");
 
  this->__linkedNode = linkedNode; //first because registerNode use linkednode to get fsobj 
  this->__fsobj = NULL;
  this->__uid = VFS::Get().registerNode(this);

  this->__childcount = 0;
  this->__at = 0;
  this->__parent = parent;
  
  if (newname == "")
    this->__name = __linkedNode->name(); 
  else
    this->__name = newname;
  this->__parent->addChild(this);
}

void		VLink::fileMapping(FileMapping *fm)
{
  this->__linkedNode->fileMapping(fm);
}

uint64_t	VLink::size(void) const
{
  return this->__linkedNode->size();
}

std::string 	VLink::linkPath(void)
{
  return this->__linkedNode->path();
}
std::string	VLink::linkName(void)
{
  return this->__linkedNode->name();
}

std::string 	VLink::linkAbsolute(void)
{
  return this->__linkedNode->absolute();
}

bool 		VLink::isFile(void) const
{
  return this->__linkedNode->isFile();
}

bool 		VLink::isDir(void) const
{
  return this->__linkedNode->isDir();
}

bool		VLink::isVDir(void) const
{
  return this->__linkedNode->isVDir();
}

bool		VLink::isDeleted(void) const
{
  return this->__linkedNode->isDeleted();
}

bool		VLink::isLink(void) const
{
  return this->__linkedNode->isLink();
}

//XXX must return NULL because it's not handled by this fsobj 
class fso*	VLink::fsobj(void) const
{
  return this->__linkedNode->fsobj();
}

Node*		VLink::linkParent()
{
  return this->__linkedNode->parent();
}

std::vector<class Node*> VLink::linkChildren()
{
  return this->__linkedNode->children();
}

bool		VLink::linkHasChildren()
{
  return this->__linkedNode->hasChildren();
}

uint32_t	VLink::linkChildCount()
{
  return this->__linkedNode->childCount();
}

Node*		VLink::linkNode()
{
  return this->__linkedNode;
}

VFile*		VLink::open()
{
  return this->__linkedNode->open();
}

bool		VLink::registerAttributes(AttributesHandler* ah)
{
  return this->__linkedNode->registerAttributes(ah);
}

AttributesHandlers&	VLink::attributesHandlers(void)
{
  return this->__linkedNode->attributesHandlers();
}


Attributes	VLink::dataType(void)
{
  return this->__linkedNode->dataType();
}

Attributes	VLink::attributes(void)
{
  return this->__linkedNode->attributes();
}

Attributes	VLink::attributesByType(uint8_t type)
{
  return this->__linkedNode->attributesByType(type);
}

std::list< Variant_p >		VLink::attributesByName(std::string name, attributeNameType tname)
{
  return this->__linkedNode->attributesByName(name, tname);
}

std::list<std::string>		VLink::attributesNames(attributeNameType tname)
{
  return this->__linkedNode->attributesNames(tname);
}

std::map<std::string, uint8_t>	VLink::attributesNamesAndTypes(void)
{
  return this->__linkedNode->attributesNamesAndTypes();
}

Attributes			VLink::dynamicAttributes(void)
{
  return this->__linkedNode->dynamicAttributes();
}

Attributes			VLink::dynamicAttributes(std::string name)
{
  return this->__linkedNode->dynamicAttributes(name);
}

std::list<std::string>		VLink::dynamicAttributesNames(void)
{
  return this->__linkedNode->dynamicAttributesNames();
}

Attributes			VLink::fsoAttributes(void)
{
 return this->__linkedNode->fsoAttributes();
}

std::string	VLink::icon(void)
{
  return this->__linkedNode->icon();
}

std::list<std::string>	VLink::compatibleModules(void)
{
  return this->__linkedNode->compatibleModules();
}

bool 	VLink::setTag(std::string name)
{
  return this->__linkedNode->setTag(name);
}

bool 	VLink::setTag(uint32_t id)
{
  return this->__linkedNode->setTag(id);
}

bool	 VLink::removeTag(std::string name)
{
  return this->__linkedNode->removeTag(name);
}

bool	 VLink::removeTag(uint32_t id)
{
  return this->__linkedNode->removeTag(id);
}

bool VLink::isTagged(uint32_t id)
{
  return this->__linkedNode->isTagged(id);
}

bool VLink::isTagged(std::string name)
{
  return this->__linkedNode->isTagged(name);
}

std::vector<Tag_p > VLink::tags()
{
  return this->__linkedNode->tags();
}

std::vector<uint32_t>	VLink::tagsId()
{
  return this->__linkedNode->tagsId();
}

VLink::~VLink()
{
}

Destruct::DValue        VLink::save(void) const
{
  DObject* vlink = Destruct::DStructs::instance().generate("DVLink");

  vlink->setValue("node", RealValue<DUnicodeString>(this->path()));
  vlink->setValue("linkNode", RealValue<DUnicodeString>(this->__linkedNode->absolute()));
  vlink->setValue("name", RealValue<DUnicodeString>(this->name()));

  return (RealValue<DObject*>(vlink));
}

VLink*                  VLink::load(Destruct::DValue const& arg)
{
  VFS& vfs =  VFS::Get();
  DObject* vlink = arg.get<DObject*>();

  std::string name = vlink->getValue("name").get<DUnicodeString>(); 
  std::string nodePath = vlink->getValue("node").get<DUnicodeString>();
  std::string linkedNodePath = vlink->getValue("linkNode").get<DUnicodeString>();
  Node* node = vfs.GetNode(nodePath);
  Node* linkedNode = vfs.GetNode(linkedNodePath);

  if (!node || !linkedNode)
    return (NULL);
  
  return (new VLink(linkedNode, node, name));
}
