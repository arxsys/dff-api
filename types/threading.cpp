/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http://www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Frederic Baguelin <fba@digital-forensic.org>
 */

#include "threading.hpp"


namespace dff
{
  Mutex::Mutex()
  {
  //std::cout << "Initializing Mutex" << std::endl;
#ifdef WIN32
 //   if (!InitializeCriticalSectionAndSpinCount(&__mutex, 0x00000400))  //XXX try smaller value or none value ?
	InitializeCriticalSection(&__mutex);
      //throw std::string("Cannot create CriticalSection");
	//std::cout << "Mutex initialized" << std::endl;
	//printf("%x\n", &__mutex); 
#else
    pthread_mutexattr_init(&__attr);
    pthread_mutexattr_settype(&__attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&__mutex, &__attr);
#endif    
  }
  
  Mutex::Mutex(const Mutex& other)
  {
#ifdef WIN32
    //if (!InitializeCriticalSectionAndSpinCount(&__mutex, 0x00000400)) //XXX
	InitializeCriticalSection(&__mutex); //XXX
     // throw std::string("Cannot create CriticalSection");
#else
    pthread_mutexattr_init(&__attr);
    pthread_mutexattr_settype(&__attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&__mutex, &__attr);    
#endif
  }
  
  Mutex::~Mutex()
  {
#ifdef WIN32
    DeleteCriticalSection(&__mutex);
#else
    pthread_mutexattr_destroy(&__attr);
    pthread_mutex_destroy(&__mutex);
#endif
  }

#ifdef WIN32
  void Mutex::lock()
  {
    EnterCriticalSection(&__mutex); 
  }

  bool Mutex::tryLock()
  {
	return TryEnterCriticalSection(&__mutex);
  }

  void Mutex::release()
  {
	LeaveCriticalSection(&__mutex);
  }
#else
  
  void	Mutex::lock()
  {
    pthread_mutex_lock(&__mutex);
  }

  bool	Mutex::tryLock()
  {
    return pthread_mutex_trylock(&__mutex) == 0;
  }

  void	Mutex::release()
  {
    pthread_mutex_unlock(&__mutex);
  }

#endif
  
  ScopedMutex::ScopedMutex(Mutex& mutex) : __mutex(mutex)
  {
    __mutex.lock();
  }

  ScopedMutex::~ScopedMutex()
  {
    __mutex.release();
  }

};