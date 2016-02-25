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

#include "vfs.hpp"
#include "node.hpp"
#include "tags.hpp"

#include "dtype.hpp"
#include "dsimpleobject.hpp"
#include "dstructs.hpp"
#include "dattribute.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp" 

namespace DFF
{

Color::Color() : r(0), g(0), b(0)
{
}

Color::Color(uint8_t cr, uint8_t cg, uint8_t cb) : r(cr), g(cg), b(cb)
{
}

Tag::Tag() : __id(0)
{
}

Tag::Tag(uint32_t id, std::string name, Color color) : __id(id), __name(name), __color(color)
{
}

Tag::Tag(uint32_t id, std::string name, uint8_t r, uint8_t g, uint8_t b) : __id(id), __name(name), __color(r, g, b)
{
}

Tag::~Tag()
{
}

const std::string Tag::name(void) const
{
  return (this->__name);
}

const Color    Tag::color(void) const
{
  return (this->__color);
}

uint32_t Tag::id(void) const
{
  return (this->__id);
}

void	Tag::setColor(Color color)
{
   this->__color = color;
}

void    Tag::setColor(uint8_t r, uint8_t g, uint8_t b)
{
  this->__color.r = r;
  this->__color.g = g;
  this->__color.b = b;
}

void	Tag::setName(const std::string name)
{
  this->__name = name;
}

#define DEFAULT_TAG(n, r, g, b) this->add(std::string(n), r, g, b);

TagsManager::TagsManager()
{
  DEFAULT_TAG("known good",   0, 255,   0)
  DEFAULT_TAG("known bad",    0,   0, 255)
  DEFAULT_TAG("malware",    255,   0,   0)
  DEFAULT_TAG("viewed",     255, 255,   0)
  DEFAULT_TAG("suspicious", 255, 85,    0)
  this->__defaults = this->__tagsList.size();
  this->__declare();
}

void    TagsManager::__declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  Destruct::DStruct*  color = new Destruct::DStruct(0, "Color", Destruct::DSimpleObject::newObject);
  color->addAttribute(Destruct::DAttribute(Destruct::DType::DUInt8Type, "r"));
  color->addAttribute(Destruct::DAttribute(Destruct::DType::DUInt8Type, "g"));
  color->addAttribute(Destruct::DAttribute(Destruct::DType::DUInt8Type, "b"));
  destruct.registerDStruct(color);

  Destruct::DStruct*  tag = new Destruct::DStruct(0, "Tag", Destruct::DSimpleObject::newObject);
  tag->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"));
  tag->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "color"));
  destruct.registerDStruct(tag);
}

TagsManager::~TagsManager()
{
  std::vector<Tag*>::iterator tag = this->__tagsList.begin();
  for (; tag != this->__tagsList.end(); ++tag)
     delete (*tag);
}

TagsManager&	TagsManager::get()
{
  static TagsManager single;
  return (single);
}

uint32_t        TagsManager::add(const std::string name, Color color)
{
  return (this->add(name, color.r, color.g, color.b));
}

uint32_t 	TagsManager::add(const std::string name, uint8_t r, uint8_t g, uint8_t b)
{
  try 
  {
    Tag* t  =  this->tag(name);
    return (t->id());
  }
  catch (envError) 
  {
  }

  if (this->__tagsList.size() < 63)
  {
    uint32_t id = this->__tagsList.size() + 1;
 
    Tag* tag = new Tag(id, name, r, g, b); 
    this->__tagsList.push_back(tag);
    return (id);
  }
  else
  {
    uint32_t  id = 0;

    for (; id < 63; id++)
    {
      if (this->__tagsList[id] == NULL)
      {
        this->__tagsList[id] = new Tag(id + 1, name, r, g, b);
	return (id + 1);
      }
    }
  }

  return (0);
}

uint32_t 	TagsManager::add(const std::string name)
{
  //getcolorauto XXX //random ??
  return (this->add(name, 100, 170, 80));
}


void            TagsManager::__removeNodesTag(uint32_t id, Node* node)
{
  node->removeTag(id);
  if (!(node->hasChildren()))
    return ;

  std::vector<Node*>           childs = node->children();
  std::vector<Node*>::iterator it = childs.begin();
  for (; it != childs.end(); it++)
    if ((*it) != NULL)
      this->__removeNodesTag(id, (*it));
}

void            TagsManager::__removeNodesTag(uint32_t id)
{
  Node* root = VFS::Get().GetNode("/");

  this->__removeNodesTag(id, root);
}

bool		TagsManager::remove(uint32_t id)
{
  try
  {
    Tag* t = this->__tagsList.at(id - 1);
    if (t != NULL)
    {
      this->__removeNodesTag(id);
      if (id > this->__defaults)
      {
        delete this->__tagsList[id - 1];
        this->__tagsList[id - 1] = NULL;
        return (true);
      }
      else
        return (false);
    }
  }
  catch (std::exception)
  {
    return (false);
  }
  return (false);
}


bool		TagsManager::remove(const std::string name)
{
  std::vector<Tag* >::iterator it = this->__tagsList.begin();
  
  for (; it != this->__tagsList.end(); it++)
  {
    if (((*it) != NULL) && ((*it)->name() == name))
      return (this->remove((*it)->id())); 
  } 
  return (false);
}

const std::vector<Tag* >	TagsManager::tags(void) const
{
  return (this->__tagsList);
}

Tag*			TagsManager::tag(uint32_t id) const
{
  try 
  {
    Tag* t = this->__tagsList.at(id - 1);
    if (t != NULL)
      return (t);
  }
  catch (std::exception)
  {
  }
  throw envError("Tag not found"); 
}

Tag*			TagsManager::tag(const std::string name) const
{
  std::vector<Tag* >::const_iterator	it = this->__tagsList.begin();

  for (; it != this->__tagsList.end(); it++)
    if (((*it) != NULL) && (*it)->name() == name)
      return (*it); 

  throw envError("Tag not found");
}

Destruct::DValue        TagsManager::save(void) const
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  Destruct::DStruct* tagStruct = destruct.find("Tag");
  Destruct::DStruct* colorStruct = destruct.find("Color");
  Destruct::DObject* vector = destruct.generate("DVectorObject");

  std::vector<Tag* >::const_iterator tag = this->__tagsList.begin();
  for (; tag != this->__tagsList.end(); tag++)
  {
    //avoid loading fixed tags ??
    Destruct::DObject* dtag = tagStruct->newObject();
    Destruct::DObject* dcolor = colorStruct->newObject();

    //dtag->setValue("id", Destruct::RealValue<DUInt32>((*tag)->id()));
    dtag->setValue("name", Destruct::RealValue<Destruct::DUnicodeString>((*tag)->name()));
    
    Color color = (*tag)->color();
    dcolor->setValue("r", Destruct::RealValue<DUInt8>(color.r));
    dcolor->setValue("g", Destruct::RealValue<DUInt8>(color.g));
    dcolor->setValue("b", Destruct::RealValue<DUInt8>(color.b));
    dtag->setValue("color", Destruct::RealValue<Destruct::DObject*>(dcolor));
    dcolor->destroy();
    vector->call("push", Destruct::RealValue<Destruct::DObject*>(dtag));
    dtag->destroy();
  }
  return (Destruct::RealValue<Destruct::DObject*>(vector));
}

void                    TagsManager::load(Destruct::DValue value)
{
  Destruct::DObject* vector = value.get<Destruct::DObject*>(); 
  DUInt64 count = vector->call("size").get<DUInt64>();
  
  for (DUInt64 index = 0; index < count; index++)
  {
    Destruct::DObject* tag = vector->call("get", Destruct::RealValue<DUInt64>(index)).get<Destruct::DObject*>();
    Destruct::DUnicodeString name = tag->getValue("name").get<Destruct::DUnicodeString>(); 
    Destruct::DObject* color = tag->getValue("color").get<Destruct::DObject*>();
    tag->destroy();
    uint8_t r = color->getValue("r").get<DUInt8>(); 
    uint8_t g = color->getValue("g").get<DUInt8>(); 
    uint8_t b = color->getValue("b").get<DUInt8>(); 
    this->add(name.string(), r, g, b); 
    color->destroy();
  }
  vector->destroy();
}

}
