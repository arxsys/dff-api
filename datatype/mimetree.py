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
#  Frederic Baguelin <fba@digital-forensic.org>
#

import os
import sys
import struct

class MimeTree():
    def __init__(self):
        self.mimetypes = {}
        if os.name == "nt":
            if hasattr(sys, "frozen"):
                self.mgc = os.path.abspath(os.path.join(os.path.dirname(sys.executable), "resources/magic.mgc"))
            else:
                self.mgc = os.path.join(sys.path[0], "dff/api/magic/magic.mgc")
        else:
            self.mgc = "/usr/share/misc/magic.mgc"
        try:
            self.mgcfile = open(self.mgc, 'rb')
        except:
            self.__err("can't open magic file")
        buff = self.mgcfile.read()
        lbuff = len(buff)
        if lbuff < 8:
            self.__err("magic file is too short")
        self.mgcsize = lbuff
        magicno, = struct.unpack("<I", buff[0:4])
        smagicno, = struct.unpack(">I", buff[0:4])
        bswap = False
        if magicno != 0xF11E041C:
            if smagicno != 0xF11E041C:
                self.__err("bad magic file")
            else:
                bswap = True
        if bswap:
            version, = struct.unpack(">I", buff[4:8])
        else:
            version, = struct.unpack("<I", buff[4:8])
        print "Working with magic version", version
        # default == version <= 7
        self.__magic_size = 232
        self.__desc_offset = 64
        self.__desc_size = 64
        self.__mime_size = 64
        if version == 8:
            self.__desc_offset = 96
        else:
            self.__magic_size = 248
            self.__desc_offset = 96
            self.__mime_size = 80
        self.__mime_offset = self.__desc_offset + self.__desc_size
        self.nmagic = (self.mgcsize / (self.__magic_size)) - 1
        self.populate(buff)


    def __err(self, msg):
        if self.mgcfile != None:
            self.mgcfile.close()
            del self.mgcfile
        raise RuntimeError(msg)


    def populate(self, buff):
        bpos = self.__magic_size
        for i in xrange(0, self.nmagic):
            desc, = struct.unpack(str(self.__desc_size) + "s",
                                  buff[bpos+self.__desc_offset:bpos+self.__desc_offset+self.__desc_size])
            mime, = struct.unpack(str(self.__mime_size) + "s",
                                  buff[bpos+self.__mime_offset:bpos+self.__mime_offset+self.__mime_size])
            if len(mime) and mime[0] != '\0':
                smime = mime.split("/", 1)
                if len(smime) == 2:
                    key = smime[0]
                    val = smime[1]
                    if not self.mimetypes.has_key(key):
                        self.mimetypes[key] = []
                    if val not in self.mimetypes[key]:
                        self.mimetypes[key].append(val)
            bpos += self.__magic_size
