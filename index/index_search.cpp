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

#include <locale>
#include <iostream>
#include <string>
#include <sstream>

#include "../include/vfs.hpp"
#include "../include/vlink.hpp"
#include "../include/index.hpp"

#include <CLucene.h> 
#include <CLucene/queryParser/MultiFieldQueryParser.h>
#include <CLucene/search/Compare.h>

AttributeIndex::AttributeIndex(std::string name, std::string query) : AttributesHandler(name)
{
  this->__query = query;
}

Attributes 	AttributeIndex::attributes(Node * node)
{
  Attributes	vm;
  
  vm["query"] = Variant_p(new Variant(this->__query));
  return vm;
}

IndexSearch::IndexSearch() : __location(".") // default location, should not be used.
{
}

IndexSearch::IndexSearch(const std::string & location)
{
  this->__location = location;
}

IndexSearch::~IndexSearch()
{
}

void	IndexSearch::exec_query(const std::string & query,
				const std::string & must_contain_query)
{
  lucene::analysis::standard::StandardAnalyzer * an = NULL;
  lucene::search::IndexSearcher * index = NULL;
  lucene::search::Query * q;
  TCHAR qq[512];
  wchar_t **fields = new wchar_t*[3];

  fields[0] = new wchar_t[5];
  fields[1] = new wchar_t[5];
  fields[2] = 0;
  STRCPY_AtoT(fields[0], "name", 5);
  STRCPY_AtoT(fields[1], "text", 5);

  uint8_t * flags = new uint8_t[2];

  flags[0] = lucene::queryParser::MultiFieldQueryParser::NORMAL_FIELD;
  flags[1] = lucene::queryParser::MultiFieldQueryParser::NORMAL_FIELD;

  if (this->__location.empty())
    return ;
  if (query.empty() && must_contain_query.empty())
    return ;
  this->__query = query;
  this->__must = must_contain_query;

  try
    {
      an = _CLNEW lucene::analysis::standard::StandardAnalyzer;
      index = _CLNEW lucene::search::IndexSearcher(this->__location.c_str());

      STRCPY_AtoT(qq, query.c_str(), 511);
      qq[query.size() >= 512 ? 512 : query.size()];
      if (must_contain_query.empty())
	q = lucene::queryParser::MultiFieldQueryParser::parse(qq, (const wchar_t **)fields,  an);
      else
	q = __getMultiSearchQuery(must_contain_query, an);
    }
  catch (CLuceneError & e)
    {
      std::cerr << "Cannot perform the search : " << e.what()
		<< "Does the index exist ?"
		<< std::endl;
      _CLDELETE(index);
      _CLDELETE(an);
      return ;
    }
  catch (std::exception & e)
    {
      std::cerr << "Cannot perform the search : " << e.what()
		<< "Does the index exist ?"
		<< std::endl;
      _CLDELETE(index);
      _CLDELETE(an);
      return ;
    }

  _tprintf(_T("parsed query : %s\n"), (const wchar_t*)q->toString());

  // if the query is not NULL and the Hits * is not NULL create VLinks and then
  // free resources before exiting.
  if (!q)
    std::cerr << "An error occured while parsing the query. Cannot proceed."
	      << std::endl;
  else
    {
      // Get hits and display results if not NULL.
      lucene::search::Hits *  h = index->search(q);
      if (!h)
	std::cerr << "An error occured while fetching results. Cannot proceed."
		  << std::endl;
      else
	{
	  __displayResults(h);
	  _CLDELETE(h);
	}
      _CLDELETE(q);
    }
  _CLDELETE(index);
  _CLDELETE(an);
}

void	IndexSearch::__displayResults(lucene::search::Hits * h)
{
  VFS &	vfs = VFS::Get();
  Node * root = vfs.root;
  Node * query = this->__newIndexation(root);

  std::cout << "Found " << h->length() << " hits." << std::endl;

  // Browse all hits.
  for (int32_t i = 0 ; i < h->length(); i++)
    {
      std::string	node_name;
      lucene::document::Document & doc = h->doc(i);
      Node *		node = NULL;

      node_name = narrow(doc.get(_T("path")));
      node = vfs.GetNode(node_name);
      if (node == NULL)
	std::cerr << "Node '" << node_name << "' does not exist." << std::endl;
      else
	new VLink(node, query, node->name());
    }

  // to refresh the gui, otherwise results never appear in the VFS.
  event * e = new event();
  e->value = Variant_p(new Variant(query));
  VFS::Get().notify(e);
}

Node *	IndexSearch::__newIndexation(Node * root)
{
  Node * query = NULL;
  Node * tmp = NULL;
  VFS &	vfs = VFS::Get();
  std::string	node_name;

  // instanciate a AttributeHandler
  AttributeIndex * attr = new AttributeIndex("index", this->__query);

  // add the query as an attribute and register it
  if (this->__query.size() > 10)
    node_name = "Results::" + this->__query.substr(0, 10) + "...";
  else
    node_name = "Results::" + this->__query;
  query = new Node(node_name);
  query->registerAttributes(attr);

  // add results in node "Searched items"
  tmp = vfs.GetNode("/Searched items");
  if (!tmp)
    return NULL;
  tmp->addChild(query);
  return query;
}

lucene::search::Query * IndexSearch::__getMultiSearchQuery(const std::string & query,
						   lucene::analysis::standard::StandardAnalyzer * an)
{
  /*
    # TODO : research onmultiple keywords. By default clucene perform a logical OR
    between the different terms of the search. The logical AND should be added.
  */
  return NULL;
}

bool		IndexSearch::deleteDoc(std::string path, std::string location)
{
  TCHAR *	w_path = (TCHAR *)operator new((path.size() + 1) * sizeof(w_path));
  
  STRCPY_AtoT(w_path, path.c_str(), path.size());
  w_path[path.size()] = 0;

  try
    {
      lucene::index::Term * doc_path
	= _CLNEW lucene::index::Term(_T("path"), w_path);

      lucene::store::Directory * directory 
	= lucene::store::FSDirectory::getDirectory(location.c_str(), false);

      lucene::index::IndexReader *  indexReader
	= lucene::index::IndexReader::open(directory);

      if (lucene::index::IndexReader::isLocked(location.c_str()))
	lucene::index::IndexReader::unlock(location.c_str());

      indexReader->deleteDocuments(doc_path);
      indexReader->close();

      _CLDELETE(indexReader);
      _CLDELETE(doc_path);
    }
  catch (CLuceneError & e)
    {
      std::cerr << "CLuceneError caught : " << e.what() << std::endl;
      return false;
    }
  return true;
}

char* IndexSearch::narrow( const wstring& str )
{
  ostringstream stm ;
  const ctype<char>& ctfacet =
    use_facet< ctype<char> >( stm.getloc() ) ;
  for( size_t i=0 ; i<str.size() ; ++i )
    stm << ctfacet.narrow( str[i], 0 ) ;
  string st = stm.str();
  char* c = new char [st.size()+1];
  strcpy(c, st.c_str());
  return c;
}
