#ifndef __RC_HPP__
#define __RC_HPP__

#include <iostream>
#include <string>
#include "export.hpp"

struct RCObjBase  {
public:
  EXPORT int ref_count() const
  {
    return refCount;
  }

  EXPORT int addref() const
  {
    return  add_ref();
  }
    
  EXPORT int delref() const
  {
          //if (ref_count() == 0 || del_ref() == 0 )
     if (del_ref() == 0)
     {
	delete this;
	return 0;
     } 
    return ref_count();
  }
    
protected:
  EXPORT RCObjBase();
  EXPORT RCObjBase(const RCObjBase& );
  EXPORT virtual ~RCObjBase() = 0;

private:

  RCObjBase& operator=(const RCObjBase& );

  friend struct RCObj;

  EXPORT int add_ref() const
  {
    return ++refCount;
  }
    
  EXPORT int del_ref() const
  {
    return --refCount;
  }
    
  mutable int refCount;
};



struct RCObj : virtual RCObjBase {
protected:
  RCObj()
  {
  }
};
/*! Reference an RCObj object
  \return The input pointer \a r
*/
template <class T>
inline
T* addref(T* r)
{ 
  return (r && r->addref() ) ? r : 0;
}
  
/*! Unreference an RCObj object.
  \return The input pointer \a r or nil if the object was deleted.
*/
template <class T>
inline
T* delref(T* r)
{ 
  return ( r && r->delref() ) ? r : 0;
}

template <class T>                     
struct RCPtr  {
  typedef T* pointer_type;
  typedef T& refernce_type;
  typedef T  value_type;  
  
  RCPtr() : pointee(0) {};
  RCPtr(T* realPtr) :pointee(realPtr) { addref(pointee);  };
  RCPtr(const RCPtr& rhs) : pointee(rhs.pointee) { addref(pointee);};

  ~RCPtr() { delref(pointee) ; };

  RCPtr& operator=(const RCPtr& rhs) { 
  if (pointee != rhs.pointee) 
  {
    delref(pointee);
    pointee = rhs.pointee;
    addref(pointee);
  }
  return *this;
};
  
  T* operator->() { return pointee; }
  T& operator*() { return *pointee; }

  const T* operator->() const { return pointee; }
  const T& operator*() const { return *pointee; }

  operator T*() { return pointee; }
  operator T&() { return *pointee; }  

  operator const T*() const { return pointee; }
  operator const T&() const { return *pointee; }  

  T* get() { return pointee; }
  T* get() const { return pointee; }

    
private:
  T* pointee;
};


#endif
