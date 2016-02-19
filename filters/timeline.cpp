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

#include <algorithm>

#include "vtime.hpp"
#include "timeline.hpp"
#include "variant.hpp"
#include "node.hpp"

#include <ctime>
#include <iostream>

namespace DFF
{

TimeLineNode::TimeLineNode(Node* node, const std::string& attributeName, const vtime& attribute) : __node(node), __attributeName(attributeName), __timeAttribute(attribute)
{
}
      
TimeLineNode::TimeLineNode(const TimeLineNode& copy) : __node(copy.__node), __attributeName(copy.__attributeName), __timeAttribute(copy.__timeAttribute)
{
}

TimeLineNode::~TimeLineNode()
{
  std::cout << "~TimeLineNode" << std::endl;
}
 
bool    TimeLineNode::compare(TimeLineNode* a, TimeLineNode* b)
{
  if (a && b)
    return (a->__timeAttribute < b->__timeAttribute);
  else
    return false;
}
 
Node*   TimeLineNode::node(void) const
{
  return (this->__node);
}

vtime   TimeLineNode::attribute(void) const
{
  return (this->__timeAttribute);
}

const std::string TimeLineNode::attributeName(void) const
{
  return (this->__attributeName);
}

/**
 *  TimeLiner
 */
TimeLine::TimeLine(std::vector<Node*> nodes) : __stop(0), __processed(0), __toProcess(0),  __nodes(nodes)
{
  std::cout << "TimeLine()" << std::endl;
}

TimeLine::~TimeLine()
{
  std::cout << "~TimeLine" << std::endl; //hum on delte et on file vector apres donc verifier que au moins c bien del les TimeLineNode() par python/swig  use //newobject ? mais c une copy pas un pointeur donc ca edvrait se faire
  //std::vector<TimeLineNode*>::iterator timeLineNode = this->__sorted.begin();
  //for(; timeLineNode != this->__sorted.end(); ++timeLineNode)
  //delete (*timeLineNode);
  //this->clear();
}

void                          TimeLine::stop(void)
{
  std::cout << "setting stop to 1" << std::endl;
  this->__stop = 1;
}

uint64_t                      TimeLine::toProcess(void) const
{
  return (this->__toProcess);
}

uint64_t                      TimeLine::processed(void) const
{
  return (this->__processed);
}

std::vector<TimeLineNode*>    TimeLine::sort(void)
{
  std::cout << "TimeLine::sort() " << this->__nodes.size() << std::endl;
  this->__toProcess = this->__nodes.size();
  std::clock_t    start = std::clock();
  std::vector<Node*>::iterator node = this->__nodes.begin();
  for (; node != this->__nodes.end(); ++node)
  {
    if (this->__stop)
    {
      std::cout << "stop is on 1 throwing" << std::endl;
      this->__sorted.clear();
      this->__stop = 0;
      throw std::string("TimeLine::sort() stopped");
    }
    Attributes attributes = (*node)->attributesByType(typeId::VTime); 
    Attributes::iterator attribute = attributes.begin();
    for (; attribute != attributes.end(); ++attribute)
    {
      vtime* time = attribute->second->value<vtime*>();
      if (time)
        this->__sorted.push_back(new TimeLineNode((*node), attribute->first, *time));
    }
    this->__processed += 1;
  }
  std::cout << "take " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl; 

  std::cout << "sorting " << this->__sorted.size() << " Nodes " << std::endl;
  start = std::clock();
  std::sort(this->__sorted.begin(), this->__sorted.end(), TimeLineNode::compare);
  std::cout << "take " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl; 

  return (this->__sorted);
}

void                    TimeLine::clear(void)
{
  this->__nodes.clear();
  this->__sorted.clear();
}

}
