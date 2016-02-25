# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Solal J. <sja@digital-forensic.org>
#
import os, sys

if hasattr(sys, "frozen"):
   from dff.api.magic import magic
else:
#    try:
#       import magic
#    except:
   from dff.api.magic import magic

from dff.api.exceptions.libexceptions import vfsError 
from dff.api.types.libtypes import Variant
from dff.api.vfs.libvfs import DataTypeHandler, DataTypeManager

class Magic(DataTypeHandler):
  def __init__(self):
     DataTypeHandler.__init__(self)
     self.__disown__()
     self.mgc_path = None
     if hasattr(sys, "frozen"):
        self.mgc_path = os.path.abspath(os.path.join(os.path.dirname(sys.executable), "resources/magic.mgc"))
     else:
        self.mgc_path = os.path.join(sys.path[0], "dff/api/magic/magic.mgc")


  def type(self, node):
    if node.size() > 0:
      mime = magic.open(magic.MAGIC_NONE)
      if self.mgc_path:
        mime.load(self.mgc_path)
      else:
        mime.load()
      filemime = None 
      try:
        f = node.open()
      except :
	return self.empty(node)
      try:
        #cannot read less than 0x2000 because of vshadow signature starting @0x1e00
        buff = f.read(0x2000)
        filemime = mime.buffer(buff)
      except Exception as e:
        print "Magic error can't read buffer:\n", e 
      finally:
        f.close()
        mime.close()	
      if filemime: 
        return filemime
      else:
	return "data"
    elif node.hasChildren():
       return "directory"
    else:
       return "empty"

magicHandler = Magic()
