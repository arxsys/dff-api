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

#ifndef __TIMELINE_HPP__
#define __TIMELINE_HPP__

#include <vector>

#include "node.hpp"

class TimeLineNode
{
public:
  TimeLineNode(void);
  TimeLineNode(Node* node, const std::string& attributeName, const vtime& time);
  TimeLineNode(const TimeLineNode& copy);
  ~TimeLineNode();
 
  bool operator<(const TimeLineNode& rhs);

  Node*                 node(void) const;
  vtime                 attribute(void) const;
  const std::string     attributeName(void) const;
private:
  Node*                 __node;
  std::string           __attributeName;
  vtime                 __timeAttribute;
};

class TimeLine
{
public:
  TimeLine(std::vector<Node*> nodes);
  
  std::vector<TimeLineNode*>   sort(void);
  void                         clear(void);
private:
  std::vector<Node*>           __nodes;
  std::vector<TimeLineNode*>   __sorted;
};

#endif
