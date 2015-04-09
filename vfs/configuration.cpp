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

