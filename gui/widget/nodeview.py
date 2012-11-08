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
#  Solal Jacob <sja@digital-forensic.org>
#  Romain Bertholon <rbe@digital-forensic.org>
# 

from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import *
from PyQt4.QtGui import *

from dff.api.vfs.libvfs import VFS

class NodeViewEvent():
  def __init__(self, parent = None):
   self.enterInDirectory = None 
   self.parent = parent
   self.VFS = VFS.Get()

  def keyReleaseEvent(self, e):
     index = self.currentIndex()
     if index.isValid():
       node = self.VFS.getNodeFromPointer(index.internalId())
       self.emit(SIGNAL("nodePressed"), e.key(), node)
     self.origView.keyReleaseEvent(self, e) 

  def mouseReleaseEvent(self, e):
     index = self.indexAt(e.pos())
     if index.isValid():
       node = self.VFS.getNodeFromPointer(index.internalId())
       self.emit(SIGNAL("nodeClicked"), e.button(), node)
     self.origView.mouseReleaseEvent(self, e)

  def mouseDoubleClickEvent(self, e):
     index = self.indexAt(e.pos())
     if index.isValid():
       node = self.VFS.getNodeFromPointer(index.internalId())
       self.emit(SIGNAL("nodeDoubleClicked"), e.button(), node) 
     self.origView.mouseReleaseEvent(self, e)

  def setEnterInDirectory(self, flag):
     self.enterInDirectory = flag  

class NodeThumbsView(QListView, NodeViewEvent):
  def __init__(self, parent):
     super(NodeThumbsView, self).__init__(parent)
     self.origView = QListView
     NodeViewEvent.__init__(self, parent)
     width = 64
     height = 64
     self.setIconGridSize(width, height)
     self.setLayoutMode(QListView.Batched)
     self.setViewMode(QListView.IconMode)

     self.setResizeMode(QListView.Adjust)
     self.setEnterInDirectory(True)
     self.setFlow(QListView.LeftToRight)
     self.setMovement(QListView.Static)
     self.setSelectionMode(QAbstractItemView.ExtendedSelection)
     self.setSelectionBehavior(QAbstractItemView.SelectRows)

     self.setBatchSize(10) 
     self.setWordWrap(False)
     self.setTextElideMode(1)	
     self.setUniformItemSizes(True)
     self.setWrapping(True)

  def setIconGridSize(self, width, height):
     self.setIconSize(QSize(width, height))
     self.setGridSize(QSize(width + 18, height + 20))

class NodeLinkTreeView(QTreeView):
  """
  This view is used to display the node tree view (in the left part of the Gui).

  Only directories and nodes having children does appear in this tree, files are not
  displayed.

  """
  def __init__(self, parent = None, coord = False):
    """
    Constructor
    """
    QTreeView.__init__(self)
    self.VFS = VFS.Get()
    self.setSelectionMode(QAbstractItemView.SingleSelection)
    self.setSelectionBehavior(QAbstractItemView.SelectItems)
    self.setUniformRowHeights(True)
    self.setSortingEnabled(False)
    self.coord = coord

  def mousePressEvent(self, e):
    """
    \reimp

    Overload of the QTreeView.mousePressEvent() event handler.

    Nodes are expanded only if users click on '+' buttons the tree. If users click on the 
    icons or names of a node, the node is not expanded.

    A nodeTreeClicked signal is emitted.

    \param e the event
    """
    index = self.indexAt(e.pos())
    if index.isValid():

      # getting node from index
      var = self.model().data(index, Qt.UserRole + 1)
      node = self.VFS.getNodeFromPointer(var.toULongLong()[0])
      if node == None:
        return

      # calculate coordinates to know if the '+' button in the tree was clicked
      self.model().nb_pop = 0
      self.insertRows(index, node)
      
      if self.coord:
        self.resizeColumnToContents(0)
      self.emit(SIGNAL("nodeTreeClicked"), e.button(), node)
      QTreeView.mousePressEvent(self, e)
 

  def mouseDoubleClickEvent(self, e):
    """
    \reimp

    When users double-click on a node in the tree view, it expands the node.

    A nodeTreeClicked signal is emitted.

    \param e the event
    """    
    self.nb_pop = 0
    index = self.indexAt(e.pos())
    node = None
    if index.isValid():

      # getting node from index
      var = self.model().data(index, Qt.UserRole + 1)
      node = self.VFS.getNodeFromPointer(var.toULongLong()[0])

      # inserting new rows
      self.insertRows(index, node)
      self.emit(SIGNAL("nodeTreeClicked"), e.button(), node)
      QTreeView.mouseDoubleClickEvent(self, e)

  def insertRows(self, index, node):
    """
    Add rows into the TreeModel. It adds only directories and nodes having children.
    """
    c_item = self.model().itemFromIndex(index)
    if c_item == None:
      return
    expanded = self.model().data(index, Qt.UserRole + 2).toBool()

    # add rows only if it has never been done before for a given index
    if expanded == False:
      row = index.row()
      item_list = []
      tmp = node.children()
      for i in tmp:
        if i.isDir() or i.hasChildren():
          new_item = QStandardItem(i.name())
          new_item.setData(long(i.this), Qt.UserRole + 1)
          new_item.setData(Qt.Unchecked, Qt.CheckStateRole)
          item_list.append(new_item)
      if len(item_list) != 0:
        c_item.appendRows(item_list)
        c_item.setData(True, Qt.UserRole + 2)

  def indexRowSizeHint(self, index):
    return 2

class NodeTreeView(QTreeView, NodeViewEvent):
  def __init__(self, parent):
     QTreeView.__init__(self, parent)
     NodeViewEvent.__init__(self, parent)
     self.origView = QTreeView
     self.setSelectionMode(QAbstractItemView.SingleSelection)
     self.setSelectionBehavior(QAbstractItemView.SelectItems)
     self.setExpandsOnDoubleClick(False)
     self.setUniformRowHeights(True)
     self.setSortingEnabled(False)

class NodeTableView(QTableView, NodeViewEvent):
  def __init__(self, parent):
    QTableView.__init__(self, parent)
    self.origView = QTableView
    NodeViewEvent.__init__(self, parent)
    self.setShowGrid(False)
    self.setEnterInDirectory(True)
    self.horizontalHeader().setStretchLastSection(True)
    self.verticalHeader().hide()
    self.setAlternatingRowColors(True)
    self.setSelectionMode(QAbstractItemView.ExtendedSelection)
    self.setSelectionBehavior(QAbstractItemView.SelectRows)
