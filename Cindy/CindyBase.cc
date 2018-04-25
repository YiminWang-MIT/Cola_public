//
// Created 1994-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyBase.cc,v 4.12 2002-12-12 12:24:41 distler Exp $
//
// Implementation of class CindyBase
//

#include "Cindy++.h"
#include "CindyNode.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <iostream>

CindyBase::CindyBase(int lvl)
{
  tree	    = NULL;
  base	    = NULL;
  level	    = NULL;
  dataroot  = NULL;
  ok_flags  = NULL;
  fastClear = getenv("CINDY_FAST_CLEAR") ? 1 : 0;
  alloc(lvl);
}

CindyBase::~CindyBase()
{
  if (tree)     delete tree;
  if (base)     delete base;
  if (level)    delete level;
  if (ok_flags) delete ok_flags;
}

void
CindyBase::alloc(int lvl)
{
  maxlevel = lvl;
  if (maxlevel > 0) {
    tree  = new CindyNode*[maxlevel];
    level = new int[maxlevel];
  }
}

void
CindyBase::alloc(int lvl, int num)
{
  if (lvl < maxlevel) {
    if (num > 0) {
      tree[lvl] = new CindyNode[num];
      level[lvl] = num;
    }
  }
}

void
CindyBase::setroot(char *ptr, int size, int longest)
{
  dataroot    = ptr;
  datasize    = size;
  longestName = longest;
  ok_flags    = new unsigned short[((size+3)>>2)<<1];
  base        = new CindyNode *[(size+1)>>1];
}

void
CindyBase::add(int lvl, int num, int up, const char *nam)
{
  tree[lvl][num].value(nam);
  if (lvl) {
    tree[lvl][num].pointer(tree[lvl-1]+up);
    tree[lvl-1][up].value(tree[lvl]+num);
  }
}

#define ADDTYPE(simpleType) \
void CindyBase::add(int lvl,int num,int up,const char *nam,simpleType *val) \
{ \
  tree[lvl][num].value(nam); \
  if (lvl) { \
    tree[lvl][num].pointer(tree[lvl-1]+up,val); \
    tree[lvl-1][up].value(tree[lvl]+num); \
  } \
  else \
    tree[lvl][num].pointer(NULL,val); \
  tree[lvl][num].rootptr(dataroot,ok_flags,base); \
}

ADDTYPE(short)
ADDTYPE(unsigned short)
ADDTYPE(int)
ADDTYPE(unsigned int)
ADDTYPE(long)
ADDTYPE(unsigned long)
ADDTYPE(float)
ADDTYPE(double)

caddr_t
CindyBase::addrOf(const char *nam)
{
    return addrOf(nam,1);
}

caddr_t
CindyBase::addrOf(const char *nam,int dmsg)
{
  CindyNode *cnode = *tree;
  char *name = new char[strlen(nam)+1];
  strcpy(name, nam);

  if (*name != '.') {		// Wenn der erste Buchstabe ein . ist,
				// dann darf der Experimentname fehlen.
    if (name != strstr(name, cnode->tellname())) {	// Vergleich mit dem
							// Experimentnamen.
      if (dmsg) std::cerr << "addrOf failed for \"" << nam
			  << "\"" << std::endl;
      delete name;
      return 0;
    }

    // delete experiment name from 'name'
    memmove(name, name+strlen(cnode->tellname()),
	    strlen(name)-strlen(cnode->tellname())+1);
  }
  if (!name[0]) {
    delete name;
    return cnode->addrOf();
  }
  while (cnode) {
    cnode = cnode->submatch(name);
    if (!name[0]) {
      delete name;
      return cnode->addrOf();
    }
  }

  delete name;
  return 0;
}

CindyBase::item_type
transType(CindyNode::Tag tag)
{
  switch (tag) {
   case CindyNode::S:
    return CindyBase::SHORT;
    break;
   case CindyNode::US:
    return CindyBase::USHORT;
    break;
   case CindyNode::I:
    return CindyBase::INT;
    break;
   case CindyNode::UI:
    return CindyBase::UINT;
    break;
   case CindyNode::L:
    return CindyBase::LONG;
    break;
   case CindyNode::UL:
    return CindyBase::ULONG;
    break;
   case CindyNode::F:
    return CindyBase::FLOAT;
    break;
   case CindyNode::D:
    return CindyBase::DOUBLE;
    break;
   default:
    return CindyBase::UNKNOWN;
    break;
  }
  return CindyBase::UNKNOWN;
}

CindyBase::item_type
CindyBase::typeOf(const char *nam,int dmsg)
{
  CindyNode *cnode = *tree;
  char *name = new char[strlen(nam)+1];
  strcpy(name, nam);

  if (*name != '.') {		// Wenn der erste Buchstabe ein . ist,
				// dann darf der Experimentname fehlen.
    if (name != strstr(name, cnode->tellname())) {	// Vergleich mit dem
							// Experimentnamen.
      if (dmsg) std::cerr << "typeOf failed for \"" << nam
			  << "\"" << std::endl;
      delete name;
      return UNKNOWN;
    }

    // delete experiment name from 'name'
    memmove(name, name+strlen(cnode->tellname()),
	    strlen(name)-strlen(cnode->tellname())+1);
  }
  if (!name[0]) {
    delete name;
    return transType(cnode->typeOf());
  }
  while (cnode) {
    cnode = cnode->submatch(name);
    if (!name[0]) {
      delete name;
      return transType(cnode->typeOf());
    }
  }

  delete name;  
  return UNKNOWN;
}

CindyItem
CindyBase::item(const char *nam,int dmsg)
{
    CindyNode *cnode = *tree;
    CindyItem citem(NULL);
    char *name = new char[strlen(nam)+1];
    strcpy(name, nam);

    if (*name != '.') {		// Wenn der erste Buchstabe ein . ist,
				// dann darf der Experimentname fehlen.
	if (name != strstr(name, cnode->tellname())) {	// Vergleich mit dem
							// Experimentnamen.
	    if (dmsg) std::cerr << "item failed for \"" << nam
				<< "\"" << std::endl;
	    delete name;
	    return citem;
	}

	// delete experiment name from 'name'
	memmove(name, name+strlen(cnode->tellname()),
		strlen(name)-strlen(cnode->tellname())+1);
    }

    if (!name[0]) {
	citem.init(cnode);
	delete name;
	return citem;
    }

    while (cnode) {
	cnode = cnode->submatch(name);
	if (!name[0]) {
	    citem.init(cnode);
	    delete name;
	    return citem;
	}
    }

    delete name;
    return citem;
}

void
CindyBase::addCallback(const char *nam, EvDpFun fun, char *data)
{
  CindyNode *cnode = *tree;
  char *name = new char[strlen(nam)+1];
  strcpy(name, nam);

  if (*name != '.') {		// Wenn der erste Buchstabe ein . ist,
				// dann darf der Experimentname fehlen.
    if (name != strstr(name, cnode->tellname())) {	// Vergleich mit dem
							// Experimentnamen.
      std::cerr << "addCallback failed for \"" << nam
		<< "\"" << std::endl;
      delete name;
      return;
    }

    // delete experiment name from 'name'
    memmove(name, name+strlen(cnode->tellname()),
	    strlen(name)-strlen(cnode->tellname())+1);
  }
  if (!name[0]) {
    cnode->addCallback(fun,data);
    delete name;
    return;
  }

  while(cnode) {
    cnode = cnode->submatch(name);
    if (!name[0]) {
      cnode->addCallback(fun,data);
      delete name;
      return;
    }
  }

  std::cerr << "addCallback failed for \"" << nam << "\"" << std::endl;
  delete name;
}

void
CindyBase::cancelCallback(const char *nam, EvDpFun fun)
{
  CindyNode *cnode = *tree;
  char *name = new char[strlen(nam)+1];
  strcpy(name, nam);

  if (*name != '.') {		// Wenn der erste Buchstabe ein . ist,
				// dann darf der Experimentname fehlen.
    if (name != strstr(name, cnode->tellname())) {	// Vergleich mit dem
							// Experimentnamen.
      std::cerr << "cancelCallback failed for \"" << nam << "\"" << std::endl;
      delete name;
      return;
    }

    // delete experiment name from 'name'
    memmove(name, name+strlen(cnode->tellname()),
	    strlen(name)-strlen(cnode->tellname())+1);
  }
  if (!name[0]) {
    cnode->cancelCallback(fun);
    delete name;
    return;
  }

  while (cnode) {
    cnode = cnode->submatch(name);
    if (!name[0]) {
      cnode->cancelCallback(fun);
      delete name;
      return;
    }
  }

  std::cerr << "cancelCallback failed for \"" << nam << "\"" << std::endl;
  delete name;
}
