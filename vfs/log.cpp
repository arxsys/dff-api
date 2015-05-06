#include "dstructs.hpp"
#include "dstruct.hpp"
#include "protocol/dmutableobject.hpp"
#include "protocol/dstream.hpp"

#include "log.hpp"
#include <ctime>

Time::Time(DStruct* dstruct, DValue const& args) : DCppObject<Time>(dstruct, args)
{
  this->init();
}

Time::Time(const Time& copy) : DCppObject<Time>(copy)
{
  this->init();
}

DUnicodeString Time::current(void)
{
  std::time_t t = std::time(NULL);
  char mbstr[100];

  if (std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&t))) 
    return (mbstr);
  return "Error";
}

Log::Log(Destruct::DStruct* dstruct, Destruct::DValue const& args) : Destruct::DCppObjectSingleton<Log>(dstruct, args)
{
  DStructs& dstructs = DStructs::instance();

  this->init();
  
  DMutableObject* arg = static_cast<DMutableObject*>(dstructs.generate("DMutable"));
  //  this->configuration() -> get log file path ? (dependance qui sera loader en premier ??)
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>("dff.log"));  //XXX ful path
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Output));
  
  this->stream = DStructs::instance().find("DStream")->newObject(RealValue<DObject*>(arg)); //File Mode XXX
  this->serializer = DStructs::instance().find("SerializeText")->newObject(this->stream); //rather iniherit ?
  this->time = DStructs::instance().find("Time")->newObject();
}

Log::Log(Log const& copy) : Destruct::DCppObjectSingleton<Log>(copy)
{
  this->init();
}

Log::~Log()
{
  std::cout << "DESTRIY LOG" << std::endl;
}

void    Log::append(Destruct::DValue const& args)
{
  //std::cout << "Appending " << args.asUnicodeString() << " to log file." << std::endl;
  //DUnicodeString msg = DTime().asUnicodeString + " : " + args.asUnicodeString;
  DUnicodeString msg = ((DObject*)this->time)->call("current").get<DUnicodeString>();
  msg += " : " + args.asUnicodeString() + "\n";
  ((DObject*)this->serializer)->call("DUnicodeString", RealValue<DUnicodeString>(msg)); //stream.write ?

  ((DObject*)this->stream)->call("flush"); 
 //XXX flush !!!

}
