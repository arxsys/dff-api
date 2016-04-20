/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2013 ArxSys
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
 *  Solal J. <sja@digital-forensic.org>
 */

#include "pyrun.swg"

%include "std_string.i"
%include "std_vector.i"
%include "windows.i"
#ifndef WIN32
%include "stdint.i"
#else
%include "wstdint.i"
#endif

%exception
{
  try
    {
      $action;
    }
  catch (DFF::vfsError &e)
    {
      SWIG_exception(SWIG_IOError, e.error.c_str());
    }
  catch (DFF::envError &e)
    {
      SWIG_PYTHON_THREAD_BEGIN_BLOCK;
      PyErr_SetString(PyExc_KeyError, e.error.c_str());
      SWIG_PYTHON_THREAD_END_BLOCK;
      return NULL;
    }
  catch (std::string e)
    {
      SWIG_exception(SWIG_RuntimeError, e.c_str());
    }
  catch (char const* cstr)
    {
      SWIG_exception(SWIG_RuntimeError, cstr);
    }
  catch (Swig::DirectorException e)
    {
      SWIG_exception(SWIG_RuntimeError, "Unknown Exception");
    }
  catch (Destruct::DException const& exception)
  {
      SWIG_exception(SWIG_RuntimeError, exception.error().c_str());
  }
}

%{
#include "../destruct/python/py_dobject.hpp"
#include <typeinfo>
%}

%typemap(in) (Destruct::DObject*) 
{
  if (PyObject_TypeCheck($input, PyDObject::pyType()))
    $1 = ((PyDObject::DPyObject*)$input)->pimpl;
  else
    $1 = Destruct::DNone;
}

%typemap(out) (Destruct::DObject*)// =>set
{
  Py_XDECREF($result);

  PyDObject::DPyObject* dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
  dobjectObject->pimpl = $1;
  dobjectObject->pimpl->addRef();
  $result = (PyObject*)dobjectObject;
}

%typemap(out) (Destruct::DObject*, Destruct::DObject*)// =>set
{
  Py_XDECREF($result);

  PyDObject::DPyObject* dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
  dobjectObject->pimpl = $1;
  dobjectObject->pimpl->addref();
  $result = (PyObject*)dobjectObject;
}

%typemap(in) (Destruct::DValue) 
{
  $1 = Destruct::DValue(PythonBaseModule::pyObjectToDValue($input));
}

//convertie un Dvalue en PyObject => set
%typemap(out) (Destruct::DValue)
{
  Py_XDECREF($result);

  $result = PythonBaseModule::dvalueAsPyObject($1);
}

