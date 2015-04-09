#ifndef __CONFIGURATION__
#define __CONFIGURATION__

#include "dstructs.hpp"
#include "protocol/dcppmutable.hpp"
#include "protocol/dmutableobject.hpp"
#include "protocol/dmutablestruct.hpp"
//#include "protocol/dstream.hpp"
//#include "protocol/dserialize.hpp"

using namespace Destruct;

/**
 *  CaseInformations is a singleton so we can handle only one case !
 */
class CaseInformations : public DCppMutable<CaseInformations>
{
public: 
  CaseInformations(Destruct::DMutableStruct* dstruct, DValue const& args) : DCppMutable<CaseInformations>(dstruct, args)
  {
    this->init();
  }

  RealValue<DUnicodeString>    casePath, caseFileName, caseInformation; 
  RealValue<DFunctionObject*> _caseFilePath;

  DUnicodeString        caseFilePath(void)
  {
    return (this->casePath + "/" + this->caseFileName + ".dfs");
  }

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (4);
  }

  static DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "casePath"), 
      DAttribute(DType::DUnicodeStringType, "caseName"),
      DAttribute(DType::DUnicodeStringType, "caseInformation"),
      DAttribute(DType::DUnicodeStringType, "caseFilePath", DType::DNoneType),
      //CaseItem ... ?
    };
    return (attributes);
  }

  static DPointer<CaseInformations>* memberBegin()
  {
    static DPointer<CaseInformations> memberPointer[] = 
    {
      DPointer<CaseInformations>(&CaseInformations::casePath),
      DPointer<CaseInformations>(&CaseInformations::caseFileName),
      DPointer<CaseInformations>(&CaseInformations::caseInformation),
      DPointer<CaseInformations>(&CaseInformations::_caseFilePath, &CaseInformations::caseFilePath),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<CaseInformations>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

  /** 
   *  CaseInformations is a singleton
   */
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
protected:
  ~CaseInformations()
  {
  }
};

/**
 *  Singleton
 */
class Configuration : public DCppMutable<Configuration> 
{
public:
  Configuration(Destruct::DMutableStruct* dstruct, DValue const& args);
  
  RealValue<DUnicodeString>    caseSavePath, defaultDumpPath; 

  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "casesSavePath"), 
      DAttribute(DType::DUnicodeStringType, "dumpsDefaultPath"),
    };
    return (attributes);
  }

  static DPointer<Configuration>* memberBegin()
  {
    static DPointer<Configuration> memberPointer[] = 
    {
      DPointer<Configuration>(&Configuration::caseSavePath),
      DPointer<Configuration>(&Configuration::defaultDumpPath),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Configuration>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

  /** 
   *  Configuration is a singleton
   */
  static DObject* newObject(Destruct::DMutableStruct* dstruct, DValue const& args)
  {
    static DObject* configuration = new Configuration(dstruct, args);
    configuration->addRef();
    return (configuration);
  }

  virtual DObject* clone() const
  {
    DObject* configuration = static_cast<DObject*>(const_cast<Configuration*>(this));
    configuration->addRef();
    return (configuration);
  }
protected:
  ~Configuration();
};

#endif
