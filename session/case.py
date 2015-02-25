import os
from time import time

from dff.api.destruct import * #Destruct, DNone, DStruct, DAttribute, DUnicodeString, *
from dff.api.taskmanager.processus import ProcessusManager
from dff.api.types.libtypes import ConfigManager, typeId, Argument, VList, Path, VMap, Variant
from dff.api.vfs.libvfs import VFS, Node, VLink
from dff.api.vfs.vfs import vfs, DataTypeManager, TagsManager
from dff.api.taskmanager.taskmanager import TaskManager

from dff.pro.api.report.manager import ReportManager

from dff.ui.gui.utils.menu import BookmarkManager, BookNode

class ArgumentsConverter(object):
  typeConverter = {
                    typeId.String : DUnicodeString,
                    typeId.CArray : DUnicodeString, #XXX Vector<DInt8>?
                    typeId.Char   : DUInt8,
                    typeId.Int16 : DInt16,
                    typeId.UInt16 : DUInt16,
                    typeId.Int32 : DInt32,
                    typeId.UInt32 : DUInt32,
                    typeId.Int64 : DInt64,
                    typeId.UInt64 : DUInt64,
                    typeId.Bool : DUInt8,
                    typeId.Map : DObject,
                    typeId.List : DObject,
                    typeId.VTime : DObject,
                    typeId.Node : DObject,
                    typeId.Path : DObject,
                    #typeId.Argument, ...
                    #typeId.VoidPtr,...
                    #typeId.VLink, ...
                  }
  def __init__(self):
    self.configManager = ConfigManager.Get()
    self.destruct = DStructs()
    self.vfs = vfs()

  def searchStruct(self, name):
     struct = self.destruct.find(name + "ArgumentsTest")
     if struct == None:
       struct = DStruct(None, name + "ArgumentsTest")
       self.createStruct(name, struct)
       self.destruct.registerDStruct(struct)
     return struct
 
  def createStruct(self, moduleName, struct):
     config = self.configManager.configByName(moduleName)
     arguments = config.arguments() #that's swig ...
     for argument in arguments:
       if argument.inputType() == Argument.List:
         dtype = DObject
       elif argument.inputType() == Argument.Empty: #??? -> bool ...
         dtype = DUInt8
       else:
         dtype = ArgumentsConverter.typeConverter[argument.type()]
       struct.addAttribute(DAttribute(argument.name(), dtype))

  def nodeToDNode(self, node):
     dnode = self.destruct.find("DNode").newObject()
     dnode.absolute = node.absolute()
     return dnode

  def pathToDPath(self, path):
     dpath = self.destruct.find("DPath").newObject()
     dpath.path = path.path
     return dpath 

  def toDObject(self, moduleName, arguments): 
     struct = self.searchStruct(moduleName)
     dobject = struct.newObject()
     #XXX inverse because of true and false this non-sense things 
     config = self.configManager.configByName(moduleName)
     configArguments = config.arguments()

     for index in range(0, struct.attributeCount()):
       attribute = struct.attribute(index)   
       name = attribute.name()
       try :
         v = arguments[name].value()
       except IndexError:
         v = None
       if type(v) == VList:
         listType = None
         for configArgument in configArguments:
           if name == configArgument.name():
             listType = configArgument.type() #typeId.Node
             break
         if listType == typeId.Node or listType == typeId.Path:
           l = self.destruct.find("DVectorObject").newObject()
         elif listType == typeId.String:
           l = self.destruct.find("DVectorString").newObject()
         else:
           print "Unknown list type can't convert to DObject : ", listType, moduleName
           raise Exception("Unknown list type can't convert to DObject")

         for varg in v:
           if listType == typeId.Node:
             res = self.nodeToDNode(varg.value())
             l.push(res)
           elif listType == typeId.Path:
             res = self.pathToDPath(varg.value())
             l.push(res)
           elif listType == typeId.String:
             l.push(varg.value())
         dvalue = l
       elif type(v) == Node:
         dvalue = self.nodeToDNode(v)
       elif type(v) == Path:
         dvalue = self.pathToDPath(v)
       else:
         for configArgument in configArguments:
           if name == configArgument.name():
             if configArgument.inputType() == Argument.Empty:
               if v == None:
                 dvalue = 0
                 break
               else:
                 dvalue = 1
                 break
             else:
               dvalue = v
       if v != None:
         try:
           setattr(dobject, name, dvalue)
         except AttributeError as e:
           print "Error setattr ", e
     return dobject
       
  def toArguments(self, moduleArguments): #return a map<String, Variant>
    vmap = VMap()
    config = self.configManager.configByName(moduleArguments.moduleName)
    configArguments = config.arguments()
    dobject = moduleArguments.arguments
    struct = dobject.instanceOf()
    for index in range(0, struct.attributeCount()):
      attribute = struct.attribute(index) #attribute.name() to be sure if index is not right sometime ? (rpc etc.. ?)   
      value = dobject.getValue(attribute.name())
      if type(value) == DObject:
        instance = value.instanceOf().name()
        if instance == "DNode":
          node = self.vfs.getnode(value.absolute)
          if node == None:
            raise Exception("Node not found ! :" + value.absolute)
          variant = Variant(node)
        elif instance == "DPath":
          #If Path does not exist popup user with path doesn't exist do you want to change it ? (and if module is connector ?)
          path = Path(value.path)
          path.thisown = False
          variant = Variant(path)
        elif instance == "DVectorObject":
          vl = VList()
          vl.thisown = False
          for subObject in value:
            subInstance = subObject.instanceOf().name()
            if subInstance == "DNode":
              node = self.vfs.getnode(subObject.absolute)
              if node == None:
                raise Exception("Node not found ! : " + subObject.absolute) 
              subVariant = Variant(node)
              subVariant.thisown = False
              vl.push_back(subVariant)
            elif subInstance == "DPath":
              path = Path(subObject.path)
              path.thisown = False
              subVariant = Variant(path)
              subVariant.thisown = False
              vl.push_back(subVariant)
            else:
              print "Unknown subobject in DVectorObject"  , subObject.instanceOf()
          variant = vl
        else:
          print "Unknown object type: " + instance
          raise Exception("DObject type : " + instance + " is not convertible to Variant type")
      else:
        #if bool/isEmpty and value is 1 : skip ...
        for carg in configArguments:
          if attribute.name() == carg.name():
            if carg.inputType() == Argument.Empty:
              if value == 0:
                value = None
              else:
                value = 1
        if value == None:
          continue 
        else: 
          if type(value) == str: # if str is '' will convert to carray, must use a type converter here for specifi int size ...
            variant = Variant(value, typeId.String)
          else:
            variant = Variant(value)
      variant.thisown = False
      vmap[attribute.name()] = variant
    vmap.thisown = False
    return vmap
 
class CaseLoader(object):
  def __init__(self):
     self.processusManager = ProcessusManager()
     self.converter = ArgumentsConverter()
     self.destruct = DStructs()
     self.sessionLoader = self.destruct.find("SessionLoader").newObject() 
     self.taskManager = TaskManager()

  def update(self, message):
     print message

  def save(self):
     filePath = DStructs().find("CaseInformations").newObject().caseFilePath()
     self.saveAs(filePath)

  def saveAs(self, filePath):
     dfs = self.destruct.find("DFS").newObject()
     self.update('Saving processus')
     self.saveProcessus(dfs)
     self.update('Saving report')
     dfs.report = ReportManager().save()
     self.update('Writing file')
     dfs.save(filePath)
     dfs = None
     self.update('Case saved')

  def saveProcessus(self, dfs):
     dfs.caseInformations = DStructs().find("CaseInformations").newObject() 
     maStruct = self.destruct.find("ModuleArguments")
     for processus in self.processusManager:
       #if processus.state != fail: #pas besoin  de garder ce qui ont fail ! (ou pas besoin de les relaunch en tt cas mais on si on rescan il va les relancer car il sont pas ds la liste des lancer)
       ma = maStruct.newObject()
       ma.moduleName = processus.mod.name #processus.mod ? ?
       ma.arguments = self.converter.toDObject(ma.moduleName, processus.args)
       try :
         ma.save = processus.save() 
       except AttributeError as e:
         print 'Error saving with save function :', e
         pass
       dfs.modules.push(ma)
 
  def load(self, filePath, connectorNewFilePath = None):
     self.update('Loading saved case from file : ' + filePath)
     #XXX cherche ds la liste de module les connector/ verifier s il les dump existes 
     #si non demander le nouveau chemin des dumps 
     dfs = self.sessionLoader.load(filePath)
     self.connectorFilesExists(dfs.modules)
     self.update('Loading datas types') #avant que les modules soit appliques ? c juste la base je suppose
     DataTypeManager.Get().load(dfs.dataType)
     self.update('Loading tags') #avant que les modules soit appliques ? c juste la base je suppose ?
     TagsManager.get().load(dfs.tags )
     self.update('Loading modules')
     self.loadModule(dfs.modules)
     self.update('Loading tree')
     dfs.loadTree()
     self.update('Loading modules arguments')
     for moduleArguments in self.loadLate:
        self.loadModuleArguments(moduleArguments) 
     self.update('Loading bookmarks')
     self.loadBookmarks(dfs.nodeTree) 
     self.update('Loading report')
     ReportManager().load(dfs.report)
     self.update('Case loaded')

  def connectorNewFilePath(self, filePath):
     return None 

  def connectorFilesExists(self, modules):
    #Check if file was moved, ask for new path and try to determine other files path based on first answer
    for moduleArguments in modules:
      if moduleArguments.moduleName == 'local': # XXX test
        for dpathIndex in range(0, moduleArguments.arguments.path.size()):
          dpath = moduleArguments.arguments.path[dpathIndex]
          if not os.path.exists(dpath.path):
            newPath = self.connectorNewFilePath(dpath.path)
            if newPath:
              newDPath = DStructs().find("DPath").newObject()
              newDPath.path = newPath
              moduleArguments.arguments.path[dpathIndex] = newDPath
            else:
              raise Exception("Can't find file " + dpath.path)

      #Check if file was moved, ask for new path and try to determine other files path based on first answer
      if moduleArguments.moduleName == 'ewf' or moduleArguments.moduleName == 'aff':
        previousPath = None
        previousValidPath = None
        for dpathIndex in range(0, moduleArguments.arguments.files.size()):
          dpath = moduleArguments.arguments.files.get(dpathIndex)
          if not os.path.exists(dpath.path):
            newPath = None
            if previousPath:
              currentPath = dpath.path[:dpath.path.rfind("/")]
              if previousPath == currentPath:
                newPath = previousValidPath + dpath.path[dpath.path.rfind("/"):]
                if not os.path.exists(newPath):
                  newPath = self.connectorNewFilePath(dpath.path)
            else: 
              newPath = self.connectorNewFilePath(dpath.path)
            if newPath:
              previousValidPath = newPath[:newPath.rfind("/")]
              newDPath = DStructs().find("DPath").newObject()
              newDPath.path = newPath
              moduleArguments.arguments.files[dpathIndex] = newDPath
            else:
              raise Exception("Can't find file " + dpath.path) 
            previousPath = dpath.path[:dpath.path.rfind("/")]

      if moduleArguments.moduleName == 'devices':
        print "Must check devices please XXX" 
        #XXX show devices boxes ????

  def loadModule(self, modules):
     self.loadLate = []
     for moduleArguments in modules:
       if moduleArguments.moduleName != "batch":  #or builtins in module.tags
         if moduleArguments.moduleName in ["hash", "clamdscan"]: #remplace par single & generic  attribute
           self.loadLate.append(moduleArguments)
         else:
           self.loadModuleArguments(moduleArguments)

  def loadModuleArguments(self, moduleArguments):
     #print 'loading ', moduleArguments.moduleName, moduleArguments.arguments, moduleArguments.save
     if moduleArguments.save != None:
       try:
         argument = self.converter.toArguments(moduleArguments)
         proc = self.taskManager.load(moduleArguments.moduleName, moduleArguments.save, ["gui", "threads"], argument)
         proc.event.wait()
       except Exception as e:
         print 'error loading ', moduleArguments.moduleName, ' with ', moduleArguments.save
         print 'error ', e
     else: #or if exception in loading saveState  with finish  XXX
       try :
         argument = self.converter.toArguments(moduleArguments)
         proc = self.taskManager.add(moduleArguments.moduleName, argument, ["gui", "threads"])
         proc.event.wait()
       except Exception as e:
         print 'error starting ', moduleArguments.moduleName
         print e 
 
  def loadBookmarks(self, nodeTree):
     bookmark = None
     case = nodeTree.children[0]# /Case/ ==> must == to Case node 
     for child in case.children:
        if child.name == "Bookmarks": #/Case/Bookmarks
          bookmark = child
          break
     if bookmark != None:
       for child in bookmark.children:
         self.createBookmark(child) 
 
  def createBookmark(self, category):
    instance = category.instanceOf().name()
    if instance == "DNode":
      BookmarkManager(None).createCategory(category.name)
      categoryNode = vfs().getnode("/Case/Bookmarks/" + category.name)
      for child in category.children:
        if child.instanceOf().name() == "VLink":
          linked = vfs().getnode(child.linkedNode)
          vl = VLink(linked, categoryNode, child.name)
          vl.__disown__()

  def showObject(self, dobject, indent = 0):
    dstruct = dobject.instanceOf()
    for index in range(0, dstruct.attributeCount()):
      attribute = dstruct.attribute(index) 
      value = dobject.getValue(index)
      print indent * ' ' + attribute.name() + " : " + str(value)
      if type(value) == DObject:
        self.showObject(value, indent + 2)
