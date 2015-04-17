#include "dstructs.hpp"
#include "dstruct.hpp"
#include "log.hpp"

Log::Log(Destruct::DStruct* dstruct, Destruct::DValue const& args) : Destruct::DCppObject<Log>(dstruct, args)
{
  // ONLY DO IT THE FIRST TIME ? IS POSSIBLE OR CREATE FUNCTION TO CHANGE STREAM
  // OR NO SINGLETON

  //opening log file
  this->init();

  this->stream = Destruct::DStructs::instance().find("DStream")->newObject(args); //File Mode XXX
  this->serializer = Destruct::DStructs::instance().find("SerializeText")->newObject(this->stream); //rather iniherit ?
}

Log::Log(const Log& copy) : Destruct::DCppObject<Log>(copy)
{
  this->init();
}

void    Log::append(Destruct::DValue const& args)
{
  std::cout << "Appending " << args.asUnicodeString() << " to log file." << std::endl;
}

//Destruct::Log() //Singleton
//{
  //DStream __myStream;
  //DSerializer  __serializer;

  //setSerializer
  //setStream ?

  //this->add();
  //this->serialize->DUnicodeStrng();
  
  
  //add();
  //{
    //DuncidoeString val;

  
   //current = DateTime();

   

   //this->serialize->DUnicodeStrng(time + current);

    //ex : 

    //[10/12/15 21:12:33] Erreur ntfs ...

  //}
  

  ////setStream(dobject);
  ////add();
  ////add(DUnicodeString message , int level);
  ////add(DObject);


  ////this->stream->

 
 

//}
