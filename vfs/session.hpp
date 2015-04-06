#ifndef __SESSIONOBJECT__
#define __SESSIONOBJECT__

#include "dstructs.hpp"
#include "protocol/dcppobject.hpp"

#include <map>

class Node;
class VFS;
class DataTypeManager;


namespace Destruct
{
  class DSerialize;
  class DStream;
}

//move destruct/debug
//nove destruct/protocol/?
class Debug
{
public:
  static void inspect(Destruct::DObject* dobject);
};

class SessionLoader : public Destruct::DCppObject<SessionLoader> //singleton ?
{
public:
  SessionLoader(Destruct::DStruct* dstruct, Destruct::DValue const& args);
  ~SessionLoader();

  static void                                   declare(void);

  Destruct::DObject*                            load(Destruct::DValue filePath);
                                                //saveDFS 
                                                //DObject configuration
                                                //DVectorObject<DFS> __cases (Multi Case)
  Destruct::RealValue<Destruct::DFunctionObject* > _load;

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (1);
  }

  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
      Destruct::DAttribute(Destruct::DType::DObjectType, "load", Destruct::DType::DUnicodeStringType),
    };
    return (attributes);
  }

  static Destruct::DPointer<SessionLoader>* memberBegin()
  {
    static Destruct::DPointer<SessionLoader> memberPointer[] = 
    {
      Destruct::DPointer<SessionLoader>(&SessionLoader::_load, &SessionLoader::load),
    };
    return (memberPointer);
  }

  static Destruct::DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static Destruct::DPointer<SessionLoader>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
private:
  Destruct::DStructs&                           __destruct;
};

class DFS : public Destruct::DCppObject<DFS> //singleton ?
{
public:
                                                   DFS(Destruct::DStruct* dstruct, Destruct::DValue const& args);
  void                                             save(Destruct::DValue const& filePath);
  void                                             loadTree(void);

  Destruct::RealValue<Destruct::DObject*>          dstructs, caseInformations, modules, dataType, tags, nodeTree, report;
  Destruct::RealValue<Destruct::DFunctionObject* > _save, _loadTree;

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (9);
  }

  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
     //VERSION attribute ?
      Destruct::DAttribute(Destruct::DType::DObjectType, "dstructs"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "caseInformations"),
      Destruct::DAttribute(Destruct::DType::DNoneType, "save", Destruct::DType::DUnicodeStringType),
      Destruct::DAttribute(Destruct::DType::DNoneType, "loadTree", Destruct::DType::DNoneType),
      Destruct::DAttribute(Destruct::DType::DObjectType, "modules"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "dataType"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "tags"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "nodeTree"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "report"),
    };
    return (attributes);
  }

  static Destruct::DPointer<DFS>* memberBegin()
  {
    static Destruct::DPointer<DFS> memberPointer[] = 
    {
      Destruct::DPointer<DFS>(&DFS::dstructs),
      Destruct::DPointer<DFS>(&DFS::caseInformations),
      Destruct::DPointer<DFS>(&DFS::_save, &DFS::save),
      Destruct::DPointer<DFS>(&DFS::_loadTree, &DFS::loadTree),
      Destruct::DPointer<DFS>(&DFS::modules),
      Destruct::DPointer<DFS>(&DFS::dataType),
      Destruct::DPointer<DFS>(&DFS::tags),
      Destruct::DPointer<DFS>(&DFS::nodeTree),
      Destruct::DPointer<DFS>(&DFS::report),
    };
    return (memberPointer);
  }

  static Destruct::DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static Destruct::DPointer<DFS >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

protected:
  ~DFS();
private:
  VFS&                  __vfs;
  Destruct::DStructs&   __destruct;
  DataTypeManager*      __dataTypeManager;
  Destruct::DObject*    __toDNodeTree(Node* node) const;
  Node*                 __getNode(const std::string& name, Node* parent) const;
  void                  __loadNode(Destruct::RealValue<Destruct::DObject*> dobject, Node* node); 
  void                  __saveDStruct(void) const;
};

#endif
