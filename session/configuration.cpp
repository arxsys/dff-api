#include "configuration.hpp"

/**
 * Configuration
 */ 
Configuration::Configuration(Destruct::DMutableStruct* dstruct, Destruct::DValue const& args) : DCppMutable<Configuration>(dstruct, args)
{
  this->init();
}

Configuration::~Configuration()
{
}

//move elswhere
/**
 *  BinaryFile
 */
BinaryFile::BinaryFile(DStruct* dstruct, DValue const& args) : DCppObject<BinaryFile>(dstruct, args), __destruct(Destruct::DStructs::instance()), __serializer(DSerializers::to("Binary")), __stream(NULL)
{
  this->init();
  this->fileName = args.get<DUnicodeString>();
  this->__openRead();
}

BinaryFile::BinaryFile(BinaryFile const& copy) : DCppObject<BinaryFile>(copy), __destruct(Destruct::DStructs::instance()), __serializer(DSerializers::to("Binary")), __stream(NULL)
{
  this->init();
}

BinaryFile::~BinaryFile()
{
  std::cout << "good bye binary file  " << std::endl; //XXX not called from python ..
  if (this->__stream)
    this->__stream->destroy();
  delete this->__serializer;
}

DValue  BinaryFile::load(void)
{
  this->__openRead();
  DValue object = this->__serializer->deserialize(*this->__stream, Destruct::DType::DObjectType);
  //this->__close();

  return (object);  
}

DValue  BinaryFile::loadDStruct(void)
{
  this->__openRead();
  DStruct* dstruct = this->__serializer->deserialize(*this->__stream);
  
  if (!dstruct)
    throw DException("Can't read dstruct.");

  this->__destruct.registerDStruct(dstruct);
  //this->__close();

  return (RealValue<DUnicodeString>(dstruct->name()));
}

void    BinaryFile::saveDStruct(DValue const& object)
{
  this->__openWrite();

  DObject* dobject = object.get<DObject*>();
  this->__serializer->serialize(*this->__stream, *dobject->instanceOf());

  dobject->destroy();
  //this->__close();
}

void    BinaryFile::save(DValue const& object)
{
  this->__openWrite();

  DObject* dobject = object.get<DObject*>();
  this->__serializer->serialize(*this->__stream, dobject);

  dobject->destroy();
  this->__close();
}

void    BinaryFile::__openRead(void)
{
  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", this->fileName); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Input));

  if (this->__stream)
    this->__stream->destroy(); 
  this->__stream = static_cast<DStream*>(this->__destruct.generate("DStream", RealValue<DObject*>(arg)));
  
  arg->destroy();
}

void    BinaryFile::__openWrite(void)
{
  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", this->fileName); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Output));

  if (this->__stream)
    this->__stream->destroy();
  this->__stream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", RealValue<DObject*>(arg)));
  arg->destroy();
}

void    BinaryFile::__close(void)
{
  this->__stream->destroy();
  this->__stream = NULL;
}

