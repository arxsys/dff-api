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

using namespace Destruct;

class Time : public DCppObject<Time>
{
public:
  Time(DStruct* dstruct, DValue const& args);
  Time(const Time& time);
  
  RealValue<DFunctionObject* > _current; 

  DUnicodeString              current(void);

  static size_t ownAttributeCount()
  {
    return (1);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "current", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<Time>* memberBegin()
  {
    static DPointer<Time> memberPointer[] = 
    {
      DPointer<Time>(&Time::_current, &Time::current),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Time>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

class Log : public DCppObjectSingleton<Log>
{
public:
  Log(DStruct* dstruct, DValue const& args);
  Log(const Log& copy);

  void          append(DValue const& args);
                                        
  RealValue<DObject*>          stream, serializer, time;
  RealValue<DFunctionObject* > _append;

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DObjectType, "stream"),
      DAttribute(DType::DObjectType, "serializer"),
      //debug
      //info
      //warn
      DAttribute(DType::DNoneType, "append", DType::DUnicodeStringType),
    };
    return (attributes);
  }

  static DPointer<Log>* memberBegin()
  {
    static DPointer<Log> memberPointer[] = 
    {
      DPointer<Log>(&Log::stream),
      DPointer<Log>(&Log::serializer),
      DPointer<Log>(&Log::_append, &Log::append),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Log >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

protected:
  ~Log();
};

#endif
