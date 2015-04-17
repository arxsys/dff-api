#ifndef __DFF_LOG__
#define __DFF_LOG__ 

#include "dstruct.hpp"
#include "protocol/dcppobject.hpp"

/**
 *  pour l 'utiliser on herite simplement :)
 *  this->registerDStruct DffLog = DStruct(Singleton, Log, "DFFLog");
 *  this->registerDSrcut  errorLog = DStruct(Singleton, Log, "ErrorLog")
 *  ou DStruct(Log, Singleton, "TestLog" ?? possible a voir ca serait sympa
 */
//class Singleton : public Destruct::DCppObject<Singleton>
//{

  ////newObject
  ////clone();
//};

class Log : public Destruct::DCppObject<Log> //singleton ?
{
public:
  Log(Destruct::DStruct* dstruct, Destruct::DValue const& args);
  Log(const Log& copy);

  void          append(Destruct::DValue const& args);
                                        
  Destruct::RealValue<Destruct::DObject*>          stream, serializer;
  Destruct::RealValue<Destruct::DFunctionObject* > _append; //add // append

/*
  static DObject* newObject(Destruct::DMutableStruct* dstruct, DValue const& args)
  {
    static DObject* configuration = new CaseInformations(dstruct, args);
    configuration->addRef();
    return (configuration);
  }

  virtual DObject* clone() const
  {
    DObject* configuration = static_cast<DObject*>(const_cast<CaseInformations*>(this));
    configuration->addRef();
    return (configuration);
  }
*/
  //XXX Singleton

//  newObject
// clone

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
      Destruct::DAttribute(Destruct::DType::DObjectType, "stream"),
      Destruct::DAttribute(Destruct::DType::DObjectType, "serializer"),
      Destruct::DAttribute(Destruct::DType::DNoneType, "append", Destruct::DType::DUnicodeStringType),
    };
    return (attributes);
  }

  static Destruct::DPointer<Log>* memberBegin()
  {
    static Destruct::DPointer<Log> memberPointer[] = 
    {
      Destruct::DPointer<Log>(&Log::stream),
      Destruct::DPointer<Log>(&Log::serializer),
      Destruct::DPointer<Log>(&Log::_append, &Log::append),
    };
    return (memberPointer);
  }

  static Destruct::DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static Destruct::DPointer<Log >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

protected:
  ~Log();
};

#endif
