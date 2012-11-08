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
 *  Romain Bertholon <rbe@digital-forensic.org>
 */

#include <iostream>

#include <CLucene.h>
#include <CLucene/util/Reader.h>
#include <CLucene/util/Misc.h>
#include <CLucene/util/dirent.h>

#include "../include/vfile.hpp"
#include "../include/index.hpp"

Index::Index()
  : __location(""), __writer(NULL), __doc(NULL), __an(NULL), __content(NULL)
{
  index_content = false;
  index_attr = false;
}

Index::Index(const std::string & location)
  : __writer(NULL), __doc(NULL), __an(NULL)
{
  this->__location = location;

  index_content = false;
  index_attr = false;
}

void	Index::setIndexContent(bool index)
{
  index_content = index;
}

void	Index::setIndexAttr(bool index)
{
  index_attr = index;
}

Index::~Index()
{
  if (this->__writer)
    _CLDELETE (this->__writer);
  if (this->__an)
    _CLDELETE (__an); 
}

bool	Index::createIndex()
{
  if (this->__location.empty())
    {
      std::cerr << "Query is empty" << std::endl;
      return false;
    }
  lucene::index::IndexWriter *	wr = NULL;
  lucene::analysis::standard::StandardAnalyzer * an = NULL;
  an = _CLNEW lucene::analysis::standard::StandardAnalyzer;
  try
    {
      if (lucene::index::IndexReader::indexExists(this->__location.c_str()))
	{
	  if (lucene::index::IndexReader::isLocked(this->__location.c_str()))
	    lucene::index::IndexReader::unlock(this->__location.c_str());
	  wr = _CLNEW lucene::index::IndexWriter(this->__location.c_str(), an, false, true);
	}
      else
	{
	  wr = _CLNEW lucene::index::IndexWriter(this->__location.c_str(), an, true, true);
	}
    }
  catch(CLuceneError &)
    {
      std::cerr << "CLuceneError exception caught. Stopping module."
		<<std::endl;
      return false;
    }
  catch(std::exception & e)
    {
      std::cerr << "Could not allocate clucene object : "
		<< e.what() << std::endl;
      return false;
    }
  wr->setMergeFactor(1000);
  wr->setMinMergeDocs(1000);
  wr->setMaxFieldLength(lucene::index::IndexWriter::DEFAULT_MAX_FIELD_LENGTH);
  this->__writer = wr;
  this->__an = an;
  return true;
}

void	Index::closeIndex()
{
  if (!this->__writer)
    return ;

  try
    {
      this->__writer->optimize();
      this->__writer->close();
      delete this->__writer;
    }
  catch(CLuceneError & e)
    {
      std::cerr << "In Index::ClauseIndex() : Caught a CLuceneError : "
		<< e.what() << std::endl;
      return ;
    }
  catch(...)
    {
    }
  this->__writer = NULL;
}

bool	Index::indexData(Node * data)
{
  TCHAR *	w_path = NULL;
  TCHAR *	w_name = NULL;

  try
    {
      lucene::document::Field * content = NULL;
      lucene::document::Field * path = NULL;
      lucene::document::Field * name = NULL;
      std::string name_lower = data->name();

      this->__doc = NULL;
      w_path = (TCHAR *)operator new((data->absolute().size() + 1) * sizeof(w_path));

      STRCPY_AtoT(w_path, data->absolute().c_str(), data->absolute().size());
      w_path[data->absolute().size()] = 0;

      this->__doc = _CLNEW lucene::document::Document();

      path = _CLNEW lucene::document::Field(_T("path"), w_path,
					    lucene::document::Field::STORE_YES
					    | lucene::document::Field::INDEX_UNTOKENIZED);

      for (unsigned int i = 0; i < name_lower.size(); ++i)
	name_lower[i] = tolower(name_lower[i]);

      w_name = (TCHAR *)operator new((data->name().size() + 1) * sizeof(w_name));
      STRCPY_AtoT(w_name, name_lower.c_str(), data->name().size());
      w_name[data->name().size()] = 0;
      name = _CLNEW lucene::document::Field(_T("name"), w_name,
					    lucene::document::Field::STORE_YES
					    | lucene::document::Field::INDEX_UNTOKENIZED);
      this->__doc->add(*name);
      if (index_content == true)
	{
	  __indexContent(data, content);
	  this->__doc->add(*__content);
	}
      this->__doc->add(*path);
      this->__writer->addDocument((this->__doc), (this->__an));
      
      delete w_path;
      delete w_name;
      _CLDELETE (this->__doc);
    }
  catch(CLuceneError & e)
    {
      std::cout << "Exception caught in Index::indexData() : "
		<< e.what() << std::endl;
      delete w_path;
      delete w_name;
      _CLDELETE (this->__doc);
      return false;
    }
  catch(std::exception & e)
    {
      std::cout << "Exception caught in Index::indexData() : "
		<< e.what() << std::endl;
      delete w_path;
      delete w_name;
      _CLDELETE (this->__doc);
      return false;
    }
  catch(...)
    {
      std::cout << "Some weird shit happened. rofl!" << std::endl;
      delete w_path;
      delete w_name;
      _CLDELETE (this->__doc);
      return false;
    }
  return true;
}

void	Index::__indexContent(Node * data, lucene::document::Field * content)
{
  VFile *	vf = NULL;
  char		buf[8193];
  TCHAR		tmp[8193];
  lucene::util::StringBuffer	str;
  unsigned int	nb_read = 0, tot_read = 0;

  str.reserve(data->size() + 1);
  vf = data->open();
  while ((nb_read = vf->read(buf, 8192)))
    {
      buf[nb_read] = 0;
      STRCPY_AtoT(tmp,buf, nb_read);
      tmp[nb_read] = 0;
      str.append(tmp);
      tot_read += nb_read;
    }
  __content = _CLNEW lucene::document::Field(_T("text"), str.getBuffer(),
				lucene::document::Field::STORE_YES
				| lucene::document::Field::INDEX_TOKENIZED);
  vf->close();
}


void	Index::addDocument(lucene::document::Document * doc)
{
  if (!this->__writer || !doc)
    return ;
  this->__writer->addDocument(doc, __an);
  this->__doc = doc;
}

lucene::document::Document *	Index::newDocument()
{

  return NULL;
}

lucene::document::Document *	Index::document() const
{
  return this->__doc;
}

void	Index::setDocument(lucene::document::Document * doc)
{
  this->__doc = doc;
}

const std::string &	Index::location() const
{
  return this->__location;
}

void	Index::setLocation(const std::string & location)
{
  this->__location = location;
}
