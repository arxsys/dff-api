# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2011 ArxSys
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

import os, shutil, time, traceback, types
from dff.api.vfs.libvfs import Node
from dff.api.exceptions.libexceptions import vfsError
from dff.api.events.libevents import EventHandler, event
from dff.api.types.libtypes import VList, Variant, RCVariant

class Extract(EventHandler):
  reservednames = ['CON', 'PRN', 'AUX', 'CLOCK$', 'NUL',
                 'COM0', 'COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9',
                 'LPT0', 'LPT1', 'LPT2', 'LPT3', 'LPT4', 'LPT5', 'LPT6', 'LPT7', 'LPT8', 'LPT9',
                 '$AttrDef', '$BadClus', '$Bitmap', '$Boot', '$LogFile', '$MFT', '$MFTMirr', 
                 'pagefile.sys', '$Secure', '$UpCase', '$Volume', '$Extend']

  max_depth = 2**31-1

  FileProgress = 0x01
  OverallProgress = 0x02
  FileFailed = 0x04
  FolderFailed = 0x08
  DataFailed = 0x10

  def __init__(self):
    EventHandler.__init__(self)
    self.__extracted = {}
    self.__errors = {}
    self.total_files = 0
    self.total_folders = 0
    self.extracted_files = 0
    self.extracted_folders = 0
    self.files_errors = 0
    self.folders_errors = 0
    self.ommited_files = 0
    self.ommited_folders = 0


  def checkFsCapacity(self):
    #perform more tests (reserved words, chars, links, ...)
    try:
      f1 = tempfile.mkstemp("casesensitive")
      f2 = tempfile.mkstemp("caseSensitive")
      f1.close()
      f2.close()
      self.case_insensitive = True
    except:
      self.case_insensitive = False
      pass

  #
  # Public Interface
  # 

  def extractFile(self, src, dst, preserve=False, overwrite=False):
    if preserve:
      dst = self.__makePreservedDirs(src, dst, overwrite)
    if type(dst) == types.UnicodeType:
      dst = dst.encode('utf-8')
    absfile, absfolder, renamed = self.__generateAbsolutePath(src, dst, overwrite)
    if absfile:
      #since requesting file extraction, if node is both file and folder, extract original filename
      if absfolder:
        absfile = absfolder
      self.total_files += 1
      self.__extractFile(src, absfile)
      return absfile
    return None


  def extractFolder(self, src, dst, preserve=False, overwrite=False):
    if preserve:
      dst = self.__makePreservedDirs(src, dst, overwrite)
    if type(dst) == types.UnicodeType:
      dst = dst.encode('utf-8')
    self.__countItems(src, False, 1)
    self.__extractTree(src, dst, overwrite, False, 1)

    
  def extractTree(self, src, dst, preserve=False, overwrite=False, extract_original=False, depth=max_depth):
    if preserve:
      dst = self.__makePreservedDirs(src, dst, overwrite)
    if type(dst) == types.UnicodeType:
      dst = dst.encode('utf-8')
    self.__countItems(src, extract_original, depth)
    self.__extractTree(src, dst, overwrite, extract_original, depth)


  def extractData(self, data, name, dst, overwrite=False):
    if type(dst) == types.UnicodeType:
      dst = dst.encode('utf-8')
    absfile, absfolder, renamed = self.__generateAbsolutePath(name, dst, overwrite)
    if absfile:
      try:
        if type(absfile) == types.UnicodeType:
          f = open(absfile.encode('utf-8'))
        else:
          f = open(absfile)
        f.write(data)
        f.close()
      except Exception:
        tb = traceback.format_exc()
        self.__notifyFailure(name, Extract.DataFailed, tb)

  #
  # Private Interface
  # 

  def __countItems(self, node, extract_original, depth):
    if node.size():
      if node.hasChildren():
        if extract_original:
          self.total_files += 1
        self.total_folders += 1
        if depth > 0:
          children = node.children()
          for child in children:
            self.__countItems(child, extract_original, depth-1)
      else:
        self.total_files += 1
    elif node.hasChildren() or node.isDir():
      self.total_folders += 1
      if node.hasChildren() and depth > 0:
        children = node.children()
        for child in children:
          self.__countItems(child, extract_original, depth-1)
    else:
      self.total_files += 1


  def __makePreservedDirs(self, node, dst, overwrite):
    npath = node.path()
    if npath == "/":
      if overwrite:
        cpath = self.__encode(node.name())
        abspath = os.path.join(dst, cpath)
        if os.path.isfile(abspath):
          os.remove(abspath)
        elif os.path.isdir(abspath):
          shutil.rmtree(abspath)
      abspath = dst
    else:
      lpath = os.path.dirname(npath[1:]).split("/")
      if overwrite:
        cpath = self.__encode(lpath[0])
        abspath = os.path.join(dst, cpath)
        if os.path.isfile(abspath):
          os.remove(abspath)
        elif os.path.isdir(abspath):
          shutil.rmtree(abspath)
      else:
        cpath, renamed = self.__generateItemName(dst, lpath[0], True)
        abspath = os.path.join(dst, cpath)
      os.mkdir(abspath)
      for path in lpath[1:]:
        path, renamed = self.__generateItemName(os.path.join(dst, cpath), path, True)
        cpath = os.path.join(cpath, path)
        abspath = os.path.join(dst, cpath)
        os.mkdir(abspath)
    return abspath


  def __generateAbsolutePath(self, node, dst, overwrite):
    if type(dst) == types.UnicodeType:
       dst = dst.encode('utf-8')
    absfile = u""
    absfolder = u""
    renamed = False
    if node.isDir() or node.hasChildren():
      if overwrite:
        if node.size():
          absfile = os.path.join(dst, self.__encode(node.name() + ".bin"))
        ename = self.__encode(node.name())
      else:
        if node.size():
          absfile, renamed = self.__generateItemName(dst, node.name() + ".bin")
          absfile = os.path.join(dst, absfile)
        ename, renamed = self.__generateItemName(dst, node.name(), True)
      absfolder = os.path.join(dst, ename)
    else:
      if overwrite:
        ename = self.__encode(node.name())
      else:
        ename, renamed = self.__generateItemName(dst, node.name())
      absfile = os.path.join(dst, ename)
    return (absfile, absfolder, renamed)


  def __extractTree(self, src, dst, overwrite, extract_original, depth):
    absfile, absfolder, renamed = self.__generateAbsolutePath(src, dst, overwrite)
    if len(absfolder):
      if len(absfile) and extract_original:
        self.__extractFile(src, absfile)
      self.__makeFolder(src, absfolder, extract_original, depth)
      if depth > 0:
        children = src.children()
        for child in children:
          self.__extractTree(child, absfolder, overwrite, extract_original, depth-1)
    elif len(absfile):
      self.__extractFile(src, absfile)
      


  def __extractFile(self, src, dst):
    self.__notifyOverallProgress()
    try:
      vfile = src.open()
      if type(dst) == types.UnicodeType:
        sysfile = open(dst.encode('utf-8'), 'wb')
      else:
        sysfile = open(dst, 'wb')
      readsize = 8192
      filesize = src.size()
      update = False
      if filesize > 10*1024*1024:
        update = True
      buff = vfile.read(readsize)
      totalread = len(buff)
      percent = 0
      while len(buff):
        sysfile.write(buff)
        buff = vfile.read(readsize)
        totalread += len(buff)
        if update and percent < round(totalread * 100 / filesize):
          percent = round(totalread * 100 / filesize)
          self.__notifyFileProgress(src, percent)
      vfile.close()
      sysfile.close()
      absnode = src.absolute()
      if type(absnode) == types.StringType:
        absnode = unicode(absnode.decode('utf-8'))
      else:
        absnode = absnode.encode('utf-8')
      #self.log["files"]["ok"] += absnode + u" --> " + dst + u"\n"
      self.extracted_files += 1
    except (vfsError, OSError, IOError):
      self.files_errors += 1
      tb = traceback.format_exc()
      self.__notifyFailure(src, Extract.FileFailed, tb)
      #self.log["files"]["nok"] += formatted_lines[-1] + "\n"
    self.__notifyOverallProgress()


  def __makeFolder(self, node, syspath, extract_original, depth):
    ret = True
    if not os.path.exists(syspath):
      try:
        os.mkdir(syspath)
        #self.log["folders"]["ok"] += syspath + "\n"
        self.extracted_folders += 1
      except OSError:
        #self.folders_errors += 1
        tb = traceback.format_exc()
        #self.log["folders"]["nok"] += formatted_lines[-1] + "\n"
        self.__notifyFailure(node, Extract.FolderFailed, tb)
        self.__countOmmited(node, extract_original, depth-1)
        ret = False
    else:
      #self.log["folders"]["ok"] += syspath + "\n"
      self.extracted_folders += 1
    self.__notifyOverallProgress()
    return ret


  def __encode(self, path):
    if type(path) == types.StringType:
      path = unicode(path.decode('utf-8'))
    else:
      path = path.encode('utf-8')
    apath = u""
    for byte in path:
      if ord(byte) <= 0x1f or byte in "\"*:<>?\/|":
        apath += "0x%0.2x" % ord(byte) 
      else:
        apath += byte
    for reserved in Extract.reservednames:
      if apath.startswith(reserved):
        apath = u"_" + apath
        break
    path = apath
    return path


  def __generateItemName(self, abspath, item, folder=False):
    count = 1
    if type(abspath) == types.StringType:
      abspath = unicode(abspath, 'utf-8')
    try:
      targets = os.listdir(abspath)
    except OSError:
      return ("", "")
    item = self.__encode(item)
    rename = False
    if folder:
      base_item = item
      ext_item = ""
    else:
      base_item, ext_item = os.path.splitext(item)
    for target in targets:
      if folder:
        base_target = target
        ext_target = ""
      else:
        base_target, ext_target = os.path.splitext(target)
      if type(base_target) == types.UnicodeType:
        base_target = base_target.encode('utf-8')
      if type(base_item) == types.UnicodeType:
        base_item = base_item.encode('utf-8')
      if type(ext_item) == types.UnicodeType:
        ext_item = ext_item.encode('utf-8')
      if base_target.lower().startswith(base_item.lower()):
        if len(base_target) == len(base_item):
          rename = True
        else:
          target_end = base_target[len(base_item):]
          if len(target_end) and target_end[0] == "_" and target_end[1:].isdigit():
            rename = True
            digit_end = int(target_end[1:])
            if digit_end > count:
              count = int(digit_end)
    if rename:
      item = base_item + "_" + str(count+1)
    else:
      item = base_item
    if len(ext_item):
      item += ext_item
    return (item, rename)


  def __notifyFileProgress(self, node, percent):
    e = event()
    e.thisown = False
    e.type = Extract.FileProgress
    vl = VList()
    vl.append(Variant(node))
    vl.append(Variant(int(percent)))
    e.value = RCVariant(Variant(vl))
    self.notify(e)


  def __notifyFailure(self, src, ftype, tb):
    e = event()
    e.thisown = False
    e.type = ftype
    vl = VList()
    if isinstance(src, Node):
      vl.append(Variant(src.absolute()))
    else:
      vl.append(Variant(src))
    vl.append(Variant(str(tb)))
    e.value = RCVariant(Variant(vl))
    self.notify(e)


  def __notifyOverallProgress(self):
    buff = ""
    if self.total_files > 0:
      percent = (float(self.extracted_files) * 100) / self.total_files
      buff += "extracted file(s):   " + str(self.extracted_files) + "/" + str(self.total_files) + " (" + str(round(percent, 2)) + "%)\n"
    if self.total_folders > 0:
      percent = (float(self.extracted_folders) * 100) / self.total_folders
      buff += "extracted folder(s): " + str(self.extracted_folders) + "/" + str(self.total_folders) + " (" + str(round(percent, 2)) + "%)\n"
    if self.files_errors > 0 and self.total_files > 0:
      percent = (float(self.files_errors) * 100) / self.total_files
      buff += "file(s) error:       " + str(self.files_errors) + "/" + str(self.total_files) + " (" + str(round(percent, 2)) + "%)\n"
    if self.folders_errors > 0 and self.total_folders > 0:
      percent = (float(self.folders_errors) * 100) / self.total_folders
      buff += "folder(s) error:     " + str(self.folders_errors) + "/" + str(self.total_folders) + " (" + str(round(percent, 2)) + "%)\n"
    if self.ommited_files > 0 and self.total_files > 0:
      percent = (float(self.ommited_files) * 100) / self.total_files
      buff += "ommited file(s):     " + str(self.ommited_files) + "/" + str(self.total_files) + " (" + str(round(percent, 2)) + "%)\n"
    if self.ommited_folders > 0 and self.total_folders > 0:
      percent = (float(self.ommited_folders) * 100) / self.total_folders
      buff += "ommited folder(s):   " + str(self.ommited_folders) + "/" + str(self.total_folders) + " (" + str(round(percent, 2)) + "%)\n"
    e = event()
    e.type = Extract.OverallProgress
    e.value = RCVariant(Variant(buff))
    e.thisown = False
    self.notify(e)


  def __countOmmited(self, node, extract_original, depth):
    if node.size():
      if node.hasChildren():
        if extract_original:
          self.ommited_files += 1
        self.ommited_folders += 1
        if depth > 0:
          children = node.children()
          for child in children:
            self.__countOmmited(child, extract_original, depth-1)
      else:
        self.ommited_files += 1
    elif node.hasChildren() or node.isDir():
      self.ommited_folders += 1
      if node.hasChildren() and depth > 0:
        children = node.children()
        for child in children:
          self.__countOmmited(child, extract_original, depth-1)
    else:
      self.ommited_files += 1
