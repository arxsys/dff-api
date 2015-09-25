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

TimeLineNode::TimeLineNode(void) : __node(NULL)
{
}

TimeLineNode::TimeLineNode(Node* node, const std::string& attributeName, Variant_p attribute) : __node(node), __attributeName(attributeName) ,__timeAttribute(attribute)
{
 this->__timeAttribute = attribute;

}
      
TimeLineNode::TimeLineNode(const TimeLineNode& copy) : __node(copy.__node), __attributeName(copy.__attributeName), __timeAttribute(copy.__timeAttribute)
{
}
 
bool    TimeLineNode::operator<(const TimeLineNode& rhs)
{
  return (this->__timeAttribute->value<vtime*>() < ((Variant_p)rhs.__timeAttribute)->value<vtime*>()); //XXX const corectness ...
}
 
Node*   TimeLineNode::node(void) const
{
  return (this->__node);
}

Variant_p  TimeLineNode::attribute(void) const
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
TimeLine::TimeLine(std::vector<Node*> nodes) : __nodes(nodes)
{
}

std::vector<TimeLineNode>    TimeLine::sort(void)
{
  std::vector<Node*>::iterator node = this->__nodes.begin();
  for (; node != this->__nodes.end(); ++node)
  {
    Attributes attributes = (*node)->attributesByType(typeId::VTime);
    Attributes::iterator attribute = attributes.begin();
    for (; attribute != attributes.end(); ++attribute)
      this->__sorted.push_back(TimeLineNode(*node, attribute->first, attribute->second));
  }

  std::sort(this->__sorted.begin(), this->__sorted.end());

  std::vector<TimeLineNode>::iterator timeLineNode = __sorted.begin();
  for (; timeLineNode != __sorted.end(); ++timeLineNode)
    std::cout << (*timeLineNode).attribute()->toString() << std::endl;

  return (this->__sorted);
}

void                    TimeLine::clear(void)
{
  this->__nodes.clear();
  this->__sorted.clear();
}
