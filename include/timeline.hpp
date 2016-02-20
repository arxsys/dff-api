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

namespace DFF
{
class Node;

class TimeLineNode
{
public:
  TimeLineNode(Node* node, const std::string& attributeName, const vtime& time);
  TimeLineNode(const TimeLineNode& copy);
  ~TimeLineNode();
 
  static bool           compare(TimeLineNode* a, TimeLineNode* b);

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
  TimeLine();
  ~TimeLine(); 

  void                                stop(void); 
  const std::vector<TimeLineNode*>&   sort(std::vector<Node*> nodes);
  const std::vector<TimeLineNode*>&   sorted(void) const;
  uint64_t                            processed(void) const;
  uint64_t                            toProcess(void) const;
private:
  void                                __clear(void);
  bool                                __stop;
  uint64_t                            __processed;
  uint64_t                            __toProcess;
  std::vector<TimeLineNode*>          __sorted;
};

}
#endif
