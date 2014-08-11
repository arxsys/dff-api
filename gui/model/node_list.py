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
#  Jeremy MOUNIER <jmo@digital-forensic.org>


from PyQt4.QtCore import SIGNAL, QAbstractItemModel, QModelIndex, QVariant, Qt, QDateTime, QString, QSize
from PyQt4.QtGui import QColor, QIcon, QPixmap, QPainter, QStandardItemModel, QStandardItem, QApplication, QCursor, QPalette

from dff.api.types.libtypes import Variant, vtime
from dff.api.events.libevents import EventHandler
from dff.api.vfs.libvfs import VFS, ABSOLUTE_ATTR_NAME, VecNode, VLink
from dff.api.types.libtypes import typeId

from dff.api.gui.thumbnail import Thumbnailer

from functools import cmp_to_key
import locale

IMAGES = ["bmp", "gif", "jpg", "jpeg", "png", "psd", "tif", "BMP", "GIF", "JPG", "JPEG", "PNG", "PSD", "TIF"]
DOCUMENT = ["doc", "docx", "odt", "DOC", "DOCX", "ODT"]
SPREADSHEET = ["xlsx", "xls", "ods", "XLSX", "XLS", "ODF"]
VIDEO = ["wmv", "mpg", "mpg4", "mov", "avi", "3gp", "3ga", "asf", "3GA", "WMV", "MOV", "MPG", "MPG4", "AVI", "3GP"]
AUDIO = ["wav", "mp3", "wma", "m4a", "aif", "mid", "mpa", "WAV", "MP3", "WMA", "M4A", "AIF", "MID", "MPA"]
ARCHIVES = ["zip", "rar", "7zip", "gz", "ZIP", "RAR", "7ZIP", "GZ"]

class NodeListModel(QAbstractItemModel):
  def __init__(self, selection):
    QAbstractItemModel.__init__(self)
    self.__list = []
    self.__rows = []
    self.__current_row = 0
    self.__row_selected = 0
    self.__thumb = True
    self.__visible_rows = 0
    self.__visible_cols = 0
    self.__recursive = False
    self.__root = None
    self.selection = selection
    if self.selection != None:
      self.connect(self.selection, SIGNAL("selectionChanged"), self.updateSelected)
    self.setDefaultAttributes()
    self.connectSignals()
    self.thumbnailer = Thumbnailer()
    self.connect(self.thumbnailer, SIGNAL("ThumbnailUpdate"), self.thumbnailUpdate)
    self.headerorder = {0:0}

  def thumbnailUpdate(self, node, pixmap):
     currentRow = self.currentRow()
     visibleRow = self.visibleRows()
     nodeList = self.list()
     currentList = nodeList[currentRow:currentRow + visibleRow]
     index = 0
     for cnode in currentList:
         if node.uid() == cnode.uid():
	   modelIndex = self.index(index, 0)
           self.emit(SIGNAL("dataChanged"), modelIndex, modelIndex)
         index += 1

  def __removeNode(self, node):
    children = node.children()
    for child in children:
      self.__removeNode(child)
    for n in self.__list:
      if n.uid() == node.uid():
        self.__list.remove(n)
        self.__row_selected = 0 
    for n in self.__rows:
      if n.uid() == node.uid():
        self.__rows.remove(n)      

  def removeNode(self, node): 
    try:
     if self.__root == None or (self.__root.path().find(node.parent().path()) != -1):
      self.__row_selected = 0 
      self.changeList(node.parent(), self.__recursive, None)
    except Exception as e :
      pass
    self.__removeNode(node)

  def vfsNotification(self, node, eventType = None):
    if eventType == 0xde1:
      pass #called by noedelistwidget
    else:
      if node.parent().uid() == self.__root.uid():
        self.changeList(self.__root, self.__recursive, self.__list[self.__row_selected])

  def updateSelected(self, count):
    self.emit(SIGNAL("layoutChanged()"))

  def connectSignals(self):
    self.connect(self, SIGNAL("appendList"), self.appendList)

  def setData(self, index, value, role):
    """
    \reimp

    Set the data which value is `value` at index `index` with role `role`.

    \return `True` if no error occured, `False` otherwise.
    """
    if not index.isValid():
      return QVariant()
    column = index.column()
    if role == Qt.CheckStateRole:
      if column == HNAME:
        node = self.VFS.getNodeById(index.internalId())
        if node == None:
          pass
        if value == Qt.Unchecked:
          if (node.uid(), 1) in self.checkedNodes:
            self.checkedNodes.remove((node.uid(), 1))
        else:
          self.checkedNodes.add((node.uid() , 1))
    QAbstractItemModel.setData(self, index, value, role)
    return True

  def changeList(self, root, recursive=False, selected=None):
    """ 
    Change the current list based on root children.
    If recursive is True, the list will be based on the recursion
    If selected is provided it will automatically be the current row
    """
    if root != None:
      self.__root = root
      self.__recursive = recursive
      self.__list = []
      self.row_selected = 0
      self.__current_row = 0
      if recursive:
        self.__fillRecursiveList(root.children())
      else:
        self.__list = root.children()
      self.sort(self.headerorder.keys()[0], self.headerorder[self.headerorder.keys()[0]])
      idx = 0
      if not recursive and selected != None:
        for i in xrange(0, len(self.__list)):
          if selected.uid() == self.__list[i].uid():
            idx = i
            break
      self.emit(SIGNAL("maximum"), len(self.__list))
      if idx > self.__current_row + self.__visible_rows:
        self.__current_row = idx
        self.select(0)
      else:
        self.select(idx)
      self.emit(SIGNAL("changeList"))

  def currentRoot(self):
    return self.__root

  def recursive(self):
    return self.__recursive

  def updateList(self, nodes, recursive=False, selected=None):
    """ 
    Update list from an existing one.
    Useful when switching from filtered view
    """
    if len(nodes):
      self.__recursive = recursive
      self.__list = nodes
      if not recursive and selected != None:
        for i in xrange(0, len(self.__list)):
          if selected.uid() == self.__list[i].uid():
            self.__current_row = i
            self.row_selected = i
            break
      self.emit(SIGNAL("maximum"), len(self.__list))
      self.select(0)
      self.emit(SIGNAL("changeList"))

  def __fillRecursiveList(self, nodes):
    for node in nodes:
      self.__list.append(node)
      if node.hasChildren():
        self.__fillRecursiveList(node.children())

  def appendList(self, node):
    """
    Append a new node to the existing model's list and emit an appended signal with the new size.
    """
    if node != None:
      try:
        self.__list.append(node)
        self.emit(SIGNAL("nodeAppended"))
        self.emit(SIGNAL("maximum"), len(self.__list))
        self.refresh(self.__current_row)
      except:
        print "Error while appending node"
        return

  def defaultAttributes(self):
    return self.__default_attributes

  def clearList(self):
    self.emit(SIGNAL("clearList"))
    self.__recursive = False
    self.__list = []
    self.__current_row = 0
    self.refresh(self.__current_row)

  def columnCount(self, index):
    attrs = self.availableAttributes()
    return len(attrs)

  def data(self, index, role):
    attributes = self.availableAttributes()
    if not index.isValid():
      return QVariant()
    if index.row() > len(self.__list) or index.row() < 0:
      return QVariant()
    try:
      node = self.__rows[index.row()]
    except:
      return QVariant()
    if role == Qt.DisplayRole :
      attrpath = str(unicode(attributes[index.column()]).encode('utf-8'))
      if attrpath == "name":
          return QVariant(QString.fromUtf8(node.name()))
      elif attrpath == "size":
          return QVariant(node.size())
      elif attrpath == "extension":
          return QVariant(QString.fromUtf8(node.extension()))
      elif attrpath == "path":
          if isinstance(node, VLink):
            return QVariant(QString.fromUtf8(node.linkPath()))
          else:
            return QVariant(QString.fromUtf8(node.path()))
      elif attrpath == "absolute":
          if isinstance(node, VLink):
            return QVariant(QString.fromUtf8(node.linkAbsolute()))
          else:
           return QVariant(QString.fromUtf8(node.absolute()))
      elif attrpath == "module":
	  if node.fsobj():
            return QVariant(QString.fromUtf8(node.fsobj().name))
          return QVariant()
      elif attrpath == "has children":
          if isinstance(node, VLink):
            return QVariant(node.linkHasChildren())
          else:
            return QVariant(node.hasChildren())
      elif attrpath == "child count":
          if isinstance(node, VLink):
            return QVariant(node.linkChildCount())
          else:
            return QVariant(node.childCount())
      elif attrpath == "is deleted":
          return QVariant(node.isDeleted())
      elif attrpath == "tags":
          #Special case tag use a delegate to draw boxes
          return QVariant()
      else:
	try :
          val = node.attributesByName(attrpath, ABSOLUTE_ATTR_NAME)
	except Exception as e:
	   print "NodeListModel data can't get attribute " + attrpath + " by name " + str(e)
	   return QVariant()
        if len(val) == 1:
          if val[0].type() == typeId.VTime:
            vtime = val[0].value()
            if vtime:
              return QVariant(str(vtime.get_time()))
          elif val[0].type() == typeId.String:
            return QVariant(QString.fromUtf8(val[0].value()))
          else:
            return QVariant(val[0].value())
        else:
          return QVariant()
    if role == Qt.ToolTipRole :
      return QVariant(QString.fromUtf8(node.name()))

    # Display icons
    if (role == Qt.DecorationRole) and (attributes[index.column()] == "name"):
      pixmap = None
      if self.__thumb:
	if self.thumbnailer.isThumbnailable(node):
	  pixmap = self.thumbnailer.generate(node)
          if pixmap is None:
	    pixmap = QPixmap(":file_temporary.png")
      if not pixmap:
        pixmap = self.getIconPixmap(node)
        if not pixmap:
          pixmap = QPixmap(node.icon())
        
        if isinstance(node, VLink):
          pixmap = pixmap.scaled(QSize(128, 128), Qt.KeepAspectRatio)
          painter = QPainter(pixmap)
          linkPixmap = QPixmap(":vlink") 
          painter.drawPixmap(0, 0, linkPixmap)
          painter.end()

	elif node.hasChildren():
          try:
            pfsobj = node.children()[0].fsobj().this
          except AttributeError:
  	    pfsobj = None
          try:
            nfsobj = node.fsobj().this
          except AttributeError:
	    nfsobj = None
          if pfsobj != nfsobj:
            pixmap = pixmap.scaled(QSize(128, 128), Qt.KeepAspectRatio)
            painter = QPainter(pixmap)
            rootPixmap = QPixmap(":root")
            painter.drawPixmap(0, 0, rootPixmap)
            painter.end()
      return QVariant(QIcon(pixmap))
	
    if role == Qt.BackgroundRole:
      if index.row() == self.activeSelection():
        palette = QPalette().color(QPalette.Highlight)
        return QVariant(QColor(palette))
    if role == Qt.ForegroundRole:
      if index.row() == self.activeSelection():
        palette = QPalette().color(QPalette.HighlightedText)
        return QVariant(QColor(palette))
      if node.isDeleted():
        return  QVariant(QColor(Qt.red))

    if (role == Qt.CheckStateRole) and (attributes[index.column()] == "name"):
      if node.uid() in self.selection.get():
        return Qt.Checked
      else:
        return Qt.Unchecked
    return QVariant()

  def setThumb(self, state):
    self.__thumb = state

  def getIconPixmap(self, node):
    ext = self.getExtension(node)
    if ext != None:
      if ext in IMAGES:
        return QPixmap(":image.png")
      elif ext in AUDIO:
        return QPixmap(":sound.png")
      elif ext == "pdf":
        return QPixmap(":pdf.png")
      elif ext in DOCUMENT:
        return QPixmap(":document.png")
      elif ext in VIDEO:
        return QPixmap(":video.png")
      elif ext in ARCHIVES:
        return QPixmap(":zip")
      elif ext in SPREADSHEET:
        return QPixmap(":spreadsheet.png")
      else:
        return None

  def getExtension(self, node):
    name = node.name()
    sext = name.split(".")
    if len(sext) > 1:
      return sext[len(sext) - 1]
    else:
      return None

  def getNode(self, row):
    try:
      node = self.__list[row]
      if node:
        return node
      else:
        return None
    except IndexError:
      return None

  def index(self, row, column, parent = QModelIndex()):
    if not self.hasIndex(row, column, parent):
     return QModelIndex()
    if not parent.isValid():
      index = self.createIndex(row, column, self.__rows[row])
      return index
    return QModelIndex()

  def parent(self, index):
    return QModelIndex()

  def refresh(self, start):
    llist = len(self.__list)
    if start < 0:
      rstart = 0
    elif (start >= llist):
      # End of list
      rstart = llist - (self.__visible_rows)
      if rstart < 0:
        rstart = 0
    # elif ((llist - start) <= self.__visible_rows + 1):
    #   rstart = self.__current_row
    #   if rstart < 0:
    #     rstart = 0
    else:
      rstart = start

    # End of List range
    if (rstart + self.__visible_rows) > len(self.__list):
      end = len(self.__list)
    else:
      end = rstart + self.__visible_rows + 1
    self.resetDisplay()
    try:
      tmplist = self.__list[rstart:end]
    except IndexError:
      return
    try:
      for nodeId in range(0, len(tmplist)):
        if tmplist[nodeId] != None:
          self.__rows.append(tmplist[nodeId])
      self.emit(SIGNAL("layoutAboutToBeChanged()"))
      self.emit(SIGNAL("layoutChanged()"))

      if self.__current_row >= 0:
        self.__current_row = rstart
      else:
        self.__current_row = 0
      self.emit(SIGNAL("current"), self.__current_row)
    except IndexError:
      print "Error while refreshing model"
      pass

  def resetDisplay(self):
    if len(self.__rows) > 0:
      self.__rows = []
      self.emit(SIGNAL("layoutAboutToBeChanged()"))
      self.emit(SIGNAL("layoutChanged()"))

  def rowCount(self, parent = None):
    return len(self.__rows)

  def currentRow(self):
    return self.__current_row

  def size(self):
    return len(self.__list)

  def setVisibleRows(self, rows):
    self.__visible_rows = rows + 1
    self.emit(SIGNAL("maximum"), len(self.__list))
    if self.__visible_rows > self.size():
      self.emit(SIGNAL("hideScroll"))
    self.refresh(self.__current_row)

  def visibleRows(self):
    return self.__visible_rows

  def seek(self, position, where = 0):
    if where == 0:
      self.refresh(position)
    elif where == 1:
      pos = self.__current_row + position
      self.refresh(pos)
    else:
      self.refresh(self.__current_row)

  def select(self, row):
    """
    Set absolute selected row id in model's list
    """
    absrow = self.__current_row + row
    try:
      node = self.__list[absrow]
      self.__row_selected = absrow
      self.refresh(self.__current_row)
      self.emit(SIGNAL("nodeListClicked"), Qt.NoButton)
      return True
    except:
      return False

  def activeSelection(self):
    """
    Return relative selected row id
    """
    return self.__row_selected - self.__current_row

  def currentNode(self):
    try:
      node = self.__list[self.__row_selected]
      return node 
    except:
      return None

  def nodeSelected(self):
    nodes = []
    nodes.append(self.__list[self.__row_selected])
    return nodes

  def setDefaultAttributes(self):
    self.__default_attributes = ["name", "size","tags", "path", "extension", "absolute", "module", "has children", "child count", "is deleted"]
    self.setSelectedAttributes(["name", "size", "tags", "path"])

  def setSelectedAttributes(self, attributes):
    self.__selected_attributes = attributes
    self.refresh(self.__current_row)

  def selectedAttributes(self):
    return self.__selected_attributes

  def availableAttributes(self):
    attrs = self.selectedAttributes()[:]
    return attrs

  def setHeaderData(self, section, orientation, value, role):
    self.emit(SIGNAL("layoutAboutToBeChanged()"))
    QAbstractItemModel.setHeaderData(self, section, orientation, value, role)
    self.emit(SIGNAL("layoutChanged()"))

  def headerData(self, section, orientation, role=Qt.DisplayRole):
    if role != Qt.DisplayRole:
      return QVariant()
    if orientation == Qt.Horizontal:
      attrs = self.availableAttributes()
      return QVariant(attrs[section])

  def sort(self, column, order):
    """
    Sort model's list and check.
    """
    self.headerorder.clear()
    self.headerorder[column] = order
    QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
    if order == Qt.DescendingOrder:
      Reverse = True
    else:
      Reverse = False
    attrs = self.availableAttributes()
    try:
      attrpath = str(unicode(attrs[column]).encode('utf-8'))
    except IndexError:
      QApplication.restoreOverrideCursor()
      self.headerorder.clear()
      self.headerorder = {0:0}
      self.refresh(0)
      self.select(0)
      return

    if isinstance(self.__list, VecNode):
      tmplist = []
      for i in range(0, len(self.__list)):
        tmplist.append(self.__list[i])
      self.__list = tmplist
    if attrpath in self.__default_attributes:
      if attrpath == "name":
        self.__list = sorted(self.__list, cmp=locale.strcoll,
                           key=lambda Node: Node.name(), 
                           reverse=Reverse)
      elif attrpath == "size":
        self.__list = sorted(self.__list,
                           key=lambda Node: Node.size(),
                           reverse=Reverse)
      elif attrpath == "extension":
        self.__list = sorted(self.__list, cmp=locale.strcoll,
                           key=lambda Node: Node.extension(),
                           reverse=Reverse)
      elif attrpath == "path":
        self.__list = sorted(self.__list, cmp=locale.strcoll,
                           key=lambda Node: Node.path(),
                           reverse=Reverse)
      elif attrpath == "absolute":
 	self.__list = sorted(self.__list, cmp=locale.strcoll,
			     key=lambda Node: Node.absolute(),
			     reverse=Reverse)
      elif attrpath == "module":
	self.__list = sorted(self.__list, cmp=self.cmp_fsobj, 
			    key=lambda Node: Node.fsobj(),
			    reverse=Reverse)
      elif attrpath == "has children":
	self.__list = sorted(self.__list, key=lambda Node: Node.hasChildren(), reverse=Reverse)
      elif attrpath == "child count":
	self.__list = sorted(self.__list, key=lambda Node: Node.childCount(), reverse=Reverse)
      elif attrpath == "is deleted": 
	self.__list = sorted(self.__list, key=lambda Node: Node.isDeleted(), reverse=Reverse)
      elif attrpath == "tags":
        self.__list = sorted(self.__list,
                             key=lambda Node: len(Node.tags()),
                             reverse=Reverse)
    else:
        self.__list = sorted(self.__list,
                             cmp=self.cmp_none, key=lambda Node: self.attributesByName(Node, attrpath, ABSOLUTE_ATTR_NAME),
                             reverse=Reverse)
    QApplication.restoreOverrideCursor()
    self.refresh(0)
    self.select(0)
    return

  def cmp_fsobj(self, x, y):
     try:
	return cmp(x.name, y.name)
     except AttributeError:
        if x == y == None:
	  return 0
        elif x == None:
	  return -1
	elif y is None:
	  return 1

  def cmp_none(self,x, y):
     try:
	return cmp(x, y)
     except TypeError: 
	if x == y == None:
	  return 0
        elif x == None:
	  return -1
	elif y is None:
	  return 1

  def attributesByName(self, node, attrpath, ABSOLUTE_ATTR_NAME):
      val = node.attributesByName(attrpath, ABSOLUTE_ATTR_NAME)
      if len(val) == 1:
        if val[0].type() == typeId.VTime:
            vtime = val[0].value()
            if vtime:
              return vtime.get_time()
        else:
            return val[0].value()

  def list(self):
    return self.__list

  def allListChecked(self):
    checked = self.selection.get()
    for node in self.__list:
      if not node.uid() in checked:
        return False
    return True

  def selectAll(self):
    for node in self.__list:
      self.selection.add(node)

  def unselectAll(self):
    for node in self.__list:
      self.selection.rm(node)
