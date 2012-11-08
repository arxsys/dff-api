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
#

from urllib import quote

from PyQt4.QtCore import SIGNAL, QBuffer, QByteArray, QIODevice, QFile, QString, QVariant, QUrl, QTimer
from PyQt4.QtGui import QPixmap, QImage, QPainter
from PyQt4.QtNetwork import QNetworkAccessManager, QNetworkReply, QNetworkRequest

from dff.api.vfs.vfs import vfs
from dff.api.vfs.iodevice import IODevice

from dff.api.gui.thumbnail import Thumbnailer

class BufferDevice(QIODevice):
  def __init__(self):
     super(QIODevice, self).__init__()
     self.qbuffer = None
     self.connect(self, SIGNAL('abouteToClose()'), self.__close)
     self.byteArray = QByteArray()
     self.qbuffer = QBuffer(self.byteArray)

  def __del__(self):
      pass

  def open(self, mode = None):
     try:
        self.qbuffer.open(QIODevice.ReadWrite | QIODevice.Unbuffered)
        self.setOpenMode(QIODevice.ReadWrite | QIODevice.Unbuffered)
	return True
     except (AttributeError, IOError):
        return False	

  def seek(self, pos):
     if self.qbuffer:
       return self.qbuffer.seek(pos)
     return False

  def __close(self):
     if self.qbuffer:
       self.qbuffer.close()
       self.qbuffer = None
     return True

  def readData(self, size):
     if self.qbuffer:
       return self.qbuffer.read(size)
     return ""

  def pos(self):
     if self.qbuffer:
       return self.qbuffer.pos()
     return 0

  def isSequential(self):
     if self.qbuffer:
       return self.qbuffer.isSequential()
     return False

  def size(self):
     return self.qbuffer.size()

  def reset(self):
     if self.qbuffer:
       self.qbuffer.seek(0)
       return True
     return False

  def atEnd(self):
     if self.qbuffer:
       return self.qbuffer.atEnd()
     return False
 
class NodeThumbnailRenderReply(QNetworkReply, BufferDevice):
  def __init__(self, parent, request):
     QNetworkReply.__init__(self, parent)
     BufferDevice.__init__(self)
     self.node = vfs().getnode(str(request.url().path().toUtf8()))
     self.thumbnailer = Thumbnailer()
     self.connect(self.thumbnailer, SIGNAL("ThumbnailUpdate"), self.updateThumbnail) 
     self.setRequest(request)
     self.setOperation(QNetworkAccessManager.GetOperation)
     mime = "image/jpg"
     self.setHeader(QNetworkRequest.ContentTypeHeader, QVariant(mime))
     self.open()
     self.setUrl(request.url())
     self.connect(parent, SIGNAL("ready"), self.ready)

  def ready(self):
     if str(self.node.dataType()).find('video') != -1:
       pixmap = self.thumbnailer.generate(self.node, iconSize = 128, frames = 10)
     else:
       pixmap = self.thumbnailer.generate(self.node, iconSize = 256, frames = 10)
     if pixmap:
       self.updateThumbnail(self.node, pixmap)

  def updateThumbnail(self, node, pixmap):
     if pixmap == None:
       pixmap = QPixmap(":file_broken.png") 
     pixmap.save(self.qbuffer, 'JPG')
     self.qbuffer.seek(0)
     self.emit(SIGNAL("readyRead()"))
     self.emit(SIGNAL("finished()")) 

  def abort(self):
     self.close()

  def __del__(self):
     self.thumbnailer.unregister()

class Resource(QIODevice):
  def __init__(self, filepath):
     super(QIODevice, self).__init__()
     self.file = None
     self.filepath = filepath
     self.connect(self, SIGNAL('aboutToClose()'), self.__close)

  def __del__(self):
     if self.file:
	self.__close()

  def open(self, mode = None):
     try:
	self.file = QFile(self)
        self.file.setFileName(QString(self.filepath))
        self.file.open(QIODevice.ReadOnly | QIODevice.Unbuffered)
        self.setOpenMode(QIODevice.ReadOnly | QIODevice.Unbuffered)
        return True
     except (AttributeError, IOError):
	return False

  def seek(self, pos):
     if self.file:
       return self.file.seek(pos)
     return  False

  def __close(self):
     if self.file:
       self.file.close()
       self.file = None	
     return True

  def readData(self, size):
     if self.file:
       return self.file.read(size)
     return ""

  def pos(self):
     if self.file:
       self.file.pos()
     return 0

  def isSequential(self):
     if self.file:
       return self.file.isSequential()
     return False

  def size(self):
     return self.file.size()

  def reset(self):
     if self.file:
       self.file.seek(0)
       return True
     return False

  def atEnd(self):
     if self.file:
       return self.file.atEnd()
     return False

class ResourceRenderReply(QNetworkReply, Resource):
  def __init__(self, parent, request):
    QNetworkReply.__init__(self, parent)
    res = ":" + str(request.url().path().toUtf8())
    Resource.__init__(self, res)
    self.setRequest(request)
    self.setOperation(QNetworkAccessManager.GetOperation)
    self.open()
    self.setUrl(request.url())
    self.connect(parent, SIGNAL("ready"), self.ready)

  def ready(self): 
    self.emit(SIGNAL("readyRead()"))
    self.emit(SIGNAL("finished()"))

  def abort(self):
     self.close()

class NodeRenderReply(QNetworkReply, IODevice):
  def __init__(self, parent, request):
     QNetworkReply.__init__(self, parent)
     node = vfs().getnode(str(request.url().path().toUtf8()))
     IODevice.__init__(self, node) 
     self.setRequest(request)
     self.setOperation(QNetworkAccessManager.GetOperation)
     mime = node.dataType()
     mime = str(mime.value()["magic mime"])
     self.setHeader(QNetworkRequest.ContentTypeHeader, QVariant(mime))
     self.open()
     self.setUrl(request.url())
     self.connect(parent, SIGNAL("ready"), self.ready)

  def ready(self):
     self.emit(SIGNAL("readyRead()"))
     self.emit(SIGNAL("finished()"))
 
  def abort(self):
     self.close()


class FramePageRenderReply(QNetworkReply, BufferDevice):
  def __init__(self, parent, request):
     QNetworkReply.__init__(self, parent)
     BufferDevice.__init__(self)
     self.__parent = parent
     self.setRequest(request)
     self.setOperation(QNetworkAccessManager.GetOperation)
     self.setHeader(QNetworkRequest.ContentTypeHeader, QVariant("text/html; charset=utf-8"))
     self.setAttribute(QNetworkRequest.CacheLoadControlAttribute, QVariant(QNetworkRequest.AlwaysNetwork))
     self.open()
     self.setUrl(request.url())
     self.connect(parent, SIGNAL("ready"), self.ready)
     self.writeHtml()
   
     QTimer.singleShot(0, self, SIGNAL("readyRead()"))
     QTimer.singleShot(0, self, SIGNAL("finished()"))

  def writeHtml(self):
     nodeurl = self.request().url().path().toUtf8()
     try:
       html = self.__parent.pages[str(nodeurl)]     
       self.qbuffer.write(html) 
     except KeyError:
       self.qbuffer.write('<html><h1>' + nodeurl + ' not found </h1></html>')
     self.qbuffer.seek(0)
 
  def ready(self):
     self.writeHtml()
     self.emit(SIGNAL("readyRead()"))
     self.emit(SIGNAL("finished()"))

  def abort(self):
     self.close()
 
class UrlRenderer(QNetworkAccessManager):
  def __init__(self, parent):
     QNetworkAccessManager.__init__(self, parent)
     self.pages = {}

  def setPages(self, pages):
     self.pages = pages 

  def createRequest(self, op, request, outgoingData):
     if op == QNetworkAccessManager.GetOperation:
       try:
         if  request.url().scheme() == "dff-node":
	   reply = NodeRenderReply(self, request)
	   return reply
         elif  request.url().scheme() == "dff-node-thumbnail":
	   reply = NodeThumbnailRenderReply(self, request)
	   return reply
         elif request.url().scheme() == "qt-resource":
	   reply = ResourceRenderReply(self, request)
	   return reply
         elif request.url().scheme() == "dff-frame-page":
	   reply =  FramePageRenderReply(self, request)
	   return reply
       except Exception as e:
 	 print 'UrlRenderer error: ' + str(e)
     return QNetworkAccessManager.createRequest(self, op, request, outgoingData) 
