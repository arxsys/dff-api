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

#include "simplefso.hpp"

namespace DFF
{
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

}
