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

#ifndef __SIMPLEFSO_HPP__
#define __SIMPLEFSO_HPP__

#include "variant.hpp"
#include "fso.hpp"

namespace DFF
{
class fso;

class SimpleFso : public fso
{
public:
  EXPORT SimpleFso(const std::string& name);

  void      start(std::map<std::string, RCPtr< Variant > > args);
  int32_t   vopen(class Node *n);
  int32_t   vread(int32_t fd, void *rbuff, uint32_t size);
  int32_t   vwrite(int32_t fd, void *wbuff, uint32_t size);
  int32_t   vclose(int32_t fd);
  uint64_t  vseek(int32_t fd, uint64_t offset, int32_t whence);
  uint32_t  status(void);
  uint64_t  vtell(int32_t fd); 
};

}
#endif
