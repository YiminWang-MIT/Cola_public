//
// Created 1993-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyNode.cc,v 4.17 2002-12-12 12:24:41 distler Exp $
//
// Implementation of class CindyNode
//

#include "CindyNode.h"
#include "CindyXDR.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <iostream>

int      CindyNode::fieldwidth = 0;
char    *CindyNode::pattern = NULL;
#ifdef HAVE_REGEX_H
regex_t *CindyNode::regex   = NULL;
#endif

CindyNode::CindyNode()
{
  name = NULL;
  up = NULL;
  v = NULL;
  tag = V;
  offset = 0;
  OK = NULL;
  callback = NULL;
  mark_ = subnodes = 0;
}

void
CindyNode::value(const char *nam)
{
  name = new char[strlen(nam)+1];
  strcpy(name, nam);
}

#ifdef HAVE_REGEX_H
void
CindyNode::setRegex(const char *rx)
{
  if (regex) {
    regfree(regex);
  } else {
    regex = new regex_t;
  }
  regcomp(regex, rx, REG_NOSUB);
}
#endif

void
CindyNode::setPattern(const char *pat)
{
  delete pattern;
  pattern = new char[strlen(pat)+1];
  strcpy(pattern, pat);
}

int
CindyNode::decodePrint(unsigned short *data)
{
  std::cout.width(6);
  std::cout << offset << ": ";
  std::cout.width(10);
  switch (tag) {
  case S:
    std::cout << (*s = *(short *)data) << std::endl;
    *OK = 1;
    return SHORTINC;
    break;
  case US:
    std::cout << (*us = *data) << std::endl;
    *OK = 1;
    return SHORTINC;
    break;
  case I:
    XDRcpy(i,data);
    std::cout << *i << std::endl;
    *OK = 1;
    return INTINC;
    break;
  case UI:
    XDRcpy(ui,data);
    std::cout << *ui << std::endl;
    *OK = 1;
    return INTINC;
    break;
  case L:
    XDRcpy(l,data);
    std::cout << *l << std::endl;
    *OK = 1;
//  return LONGINC;
    return INTINC;
    break;
  case UL:
    XDRcpy(ul,data);
    std::cout << *ul << std::endl;
    *OK = 1;
//  return LONGINC;
    return INTINC;
    break;
  case F:
    XDRcpy(f,data);
    std::cout << *f << std::endl;
    *OK = 1;
    return FLOATINC;
    break;
  case D:
    XDRcpy(d,data);
    std::cout << *d << std::endl;
    *OK = 1;
    return DOUBLEINC;
    break;
  default:
    break;
  }

  return 0;
}

inline int
CindyNode::analyse(unsigned short *data)
{
  switch (tag) {
  case S:
    *s = *(short *)data;
    *OK = 1;
    if (callback) callback->execCallback(data,SHORTINC);
    return SHORTINC;
    break;
  case US:
    *us = *data;
    *OK = 1;
    if (callback) callback->execCallback(data,SHORTINC);
    return SHORTINC;
    break;
  case I:
    XDRcpy(i,data);
    *OK = 1;
    if (callback) callback->execCallback(data,INTINC);
    return INTINC;
    break;
  case UI:
    XDRcpy(ui,data);
    *OK = 1;
    if (callback) callback->execCallback(data,INTINC);
    return INTINC;
    break;
  case L:
    XDRcpy(l,data);
    *OK = 1;
//  if (callback) callback->execCallback(data,LONGINC);
//  return LONGINC;
    if (callback) callback->execCallback(data,INTINC);
    return INTINC;
    break;
  case UL:
    XDRcpy(ul,data);
    *OK = 1;
//  if (callback) callback->execCallback(data,LONGINC);
//  return LONGINC;
    if (callback) callback->execCallback(data,INTINC);
    return INTINC;
    break;
  case F:
    XDRcpy(f,data);
    *OK = 1;
    if (callback) callback->execCallback(data,FLOATINC);
    return FLOATINC;
    break;
  case D:
    XDRcpy(d,data);
    *OK = 1;
    if (callback) callback->execCallback(data,DOUBLEINC);
    return DOUBLEINC;
    break;
  default:
    break;
  }

  return 0;
}

int
CindyNode::decodePrint(unsigned short *data, const char *lname)
{
  if (pattern != NULL) {
    if (!strstr(lname, pattern))
      return analyse(data);
#ifdef HAVE_REGEX_H
  } else if (regex != NULL) {
    if (regexec(regex, lname, 0, NULL, 0))
      return analyse(data);
#endif
  }

  std::cout.width(CindyNode::fieldwidth);
  std::cout.setf(std::ios::left,std::ios::adjustfield);
  std::cout << lname << ':';
  std::cout.width(decodeFieldWidth-13-CindyNode::fieldwidth);
  std::cout.setf(std::ios::right,std::ios::adjustfield);
  switch (tag) {
  case S:
    std::cout << (*s = *(short *)data) << "      0x";
    std::cout.fill('0');
    std::cout.width(4);
    std::cout << std::hex << *data << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
    return SHORTINC;
    break;
  case US:
    std::cout << (*us = *data) << "      0x";
    std::cout.fill('0');
    std::cout.width(4);
    std::cout << std::hex << *data << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
    return SHORTINC;
    break;
  case I:
    XDRcpy(i,data);
    std::cout << *i << "  0x";
    std::cout.fill('0');
    std::cout.width(8);
    std::cout << std::hex << *(unsigned int *)i << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
    return INTINC;
    break;
  case UI:
    XDRcpy(ui,data);
    std::cout << *ui << "  0x";
    std::cout.fill('0');
    std::cout.width(8);
    std::cout << std::hex << *ui << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
    return INTINC;
    break;
  case L:
    XDRcpy(l,data);
    std::cout << *l << "  0x";
    std::cout.fill('0');
    std::cout.width(8);
    std::cout << std::hex << *(unsigned long *)l << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
//  return LONGINC;
    return INTINC;
    break;
  case UL:
    XDRcpy(ul,data);
    std::cout << *ul << "  0x";
    std::cout.fill('0');
    std::cout.width(8);
    std::cout << std::hex << *ul << std::dec << std::endl;
    std::cout.fill(' ');
    *OK = 1;
//  return LONGINC;
    return INTINC;
    break;
  case F:
    XDRcpy(f,data);
    std::cout << *f << std::endl;
    *OK = 1;
    return FLOATINC;
    break;
  case D:
    XDRcpy(d,data);
    std::cout << *d << std::endl;
    *OK = 1;
    return DOUBLEINC;
    break;
  default:
    break;
  }

  return 0;
}

struct dhead {
  short num;
  short len;
};

static unsigned short *Top[32];

int
CindyNode::fastAnalyse(unsigned short *d, int len)
{
  register unsigned short *data = d;

  if (tag != C)
    return analyse(data);

  if (callback)
    callback->execCallback(data,len);

  *Top = data + len;
  register CindyNode *node = this;
  register struct dhead *dh;
  unsigned short *t, **top = Top;
  CindyNode *dnode;

  while (data < *Top) {
    dh = (struct dhead *) data;
    data += 2;
    if (dh->num < 0) {
      if (dh->len < 0) {	// compact data
	t = data - dh->len;
	dnode = node->c - dh->num - 1;
	if (dnode->callback)
	  dnode->callback->execCallback(data, -dh->len);
	dnode = dnode->c - 1;
	while (data < t)
	  data += (dnode+(*data++))->analyse(data);
      }
      else {			// next header
	node = node->c - dh->num - 1;
	*(++top) = data + dh->len;
	if (*top <= top[-1]) {
	  if (node->callback)
	    node->callback->execCallback(data, dh->len);
	  continue;
	} else {
	  data = *top = top[-1];
	  std::cerr << "ERROR: corrupt data - event : " << d[-2] << std::endl;
	}
      }
    }
    else {			// normal data
      t = data + dh->len;
      dnode = node->c + dh->num - 1;
      if (dnode->tag == C) {
	if (dnode->callback)
	  dnode->callback->execCallback(data, dh->len);
	dnode = dnode->c;
	if (dnode->callback && (dnode->tag == C))
	  dnode->callback->execCallback(data, dh->len);
      }
      while (data < t)
	data += (dnode++)->analyse(data);
    }
    if (data >= *Top) break;
    while (data >= *top) {
      node = node->up;
      top--;
    }
  }

  return len;
}

int
CindyNode::analyse(unsigned short *data, int len)
{
  if (tag != C)
    return analyse(data);

  if (callback) {
    callback->execCallback(data,len);
  }

  unsigned short *end = data + len;
  struct dhead *dh;

  while (data < end) {
    dh = (struct dhead *) data;
    data += 2;
    if (dh->num < 0) {
      if (dh->len < 0)
	data += c[-dh->num-1].analyse(dh->len,data);
      else
	data += c[-dh->num-1].analyse(data,dh->len);
    }
    else
      data += c[dh->num-1].analyse(dh->len,data);
  }

  return (len > 0) ? len : -len;
}

int
CindyNode::analyse(int len, unsigned short *data)
{
  int i, j, k;

  if (callback && (tag == C)) {
    callback->execCallback(data,len);
  }

  if (len<0) {
    for (i=0 ; i<(-len) ; i++,data++ ) {	// compact data
      j = c[*data-1].analyse(data+1,1);
      i += j;
      data += j;
    }
    return -len;
  }

  if (tag != C)
    return analyse(data);

  for (i=j=0 ; j<len ; i++) {
    k = c[i].analyse(data+j,1);
    j += k;
  }

  return len;
}    

int
CindyNode::decode(unsigned short *data, int len)
{
  if (tag != C)
    return decodePrint(data);

  unsigned short *end = data + len;
  struct dhead *dh;

  while (data < end) {
    dh = (struct dhead *) data;
    data += 2;
    if (data+((dh->len > 0) ? dh->len : -dh->len) > end) {
      std::cerr << "range checking error" << std::endl;
    }
    if (dh->num < 0) {
      if (dh->len < 0)
	data += c[-dh->num-1].decode(dh->len,data);
      else
	data += c[-dh->num-1].decode(data,dh->len);
    }
    else
      data += c[dh->num-1].decode(dh->len,data);
  }

  return (len > 0) ? len : -len;
}

int
CindyNode::decode(int len, unsigned short *data)
{
  int i, j, k;

  if (len<0) {
    for (i=0 ; i<(-len) ; i++,data++ ) {	// compact data
      j = c[*data-1].decode(data+1,1);
      i += j;
      data += j;
    }
    return -len;
  }

  if (tag != C)
    return decodePrint(data);

  for (i=j=0 ; j<len ; i++) {
    k = c[i].decode(data+j,1);
    j += k;
  }

  return len;
}    

int
CindyNode::decode(unsigned short *data, int len, const char *prefix)
{
  char *lname = new char[strlen(prefix)+strlen(name)+1];
  strcpy(lname, prefix); strcat(lname, name);

  if (tag != C) {
    int result = decodePrint(data,lname);
    delete lname;
    return result;
  }

  unsigned short *end = data + len;
  struct dhead *dh;

  while (data < end) {
    dh = (struct dhead *) data;
    data += 2;
    if (data+((dh->len > 0) ? dh->len : -dh->len) > end) {
      std::cerr << "range checking error" << std::endl;
    }
    if (dh->num < 0) {
      if (dh->len < 0)
	data += c[-dh->num-1].decode(dh->len,data,lname);
      else
	data += c[-dh->num-1].decode(data,dh->len,lname);
    }
    else
      data += c[dh->num-1].decode(dh->len,data,lname);
  }

  delete lname;

  return (len > 0) ? len : -len;
}

int
CindyNode::decode(int len, unsigned short *data, const char *prefix)
{
  int i, j, k;
  char *lname = new char[strlen(prefix)+strlen(name)+1];
  strcpy(lname, prefix); strcat(lname, name);

  if (len<0) {
    for (i=0 ; i<(-len) ; i++,data++ ) {	// compact data
      j = c[*data-1].decode(data+1,1,lname);
      i += j;
      data += j;
    }
    delete lname;
    return -len;
  }

  if (tag != C) {
    int result = decodePrint(data,lname);
    delete lname;
    return result;
  }

  for (i=j=0 ; j<len ; i++) {
    k = c[i].decode(data+j,1,lname);
    j += k;
  }

  delete lname;
  return len;
}    

void
CindyNode::rootptr(char *root, unsigned short *ok_root, CindyNode **base)
{
  offset = (char *)v - root;
  OK = ok_root + (offset>>1);
  base[offset>>1] = this;
}

CindyNode *
CindyNode::submatch(char *N)
{
    size_t lmax = 0;
    int    imax = -1;

    for (int i=0; i<subnodes; i++)
	if (N == strstr(N, c[i].tellname()))
	    if (lmax < strlen(c[i].tellname())) {
		lmax = strlen(c[i].tellname());
		imax = i;
	    }

    if (imax < 0) return NULL;

    // delete matching string from N
    memmove(N, N+lmax, strlen(N)-lmax+1);

    return c+imax;
}

CindyNode::~CindyNode()
{
  CindyCallback *ptr, *nptr;

  if (callback) {
    ptr = callback;
    nptr = ptr->getNext();
    while (ptr) {
      delete ptr;
      ptr = nptr;
      nptr = ptr->getNext();
    }
  }

#ifdef HAVE_REGEX_H
  if (regex != NULL) delete regex;
#endif
  if (pattern != NULL) delete pattern;
}

CindyCallback::CindyCallback(EvDpFun f, char *d)
{
  fun  = f;
  data = d;
  next = NULL;
  prev = NULL;
}

CindyCallback::CindyCallback(EvDpFun f, char *d, CindyCallback *t)
{
  fun  = f;
  data = d;
  next = NULL;
  if (t) {
    while (t->next)
      t = t->next;
    t->next = this;
    prev = t;
  }
  else prev = NULL;
}

CindyCallback *
CindyCallback::cancel(EvDpFun f)
{
  CindyCallback *ccb, *ccfirst = this;

  // find first callback
  while (ccfirst->prev) ccfirst = ccfirst->prev;

  // find callback function f
  ccb = ccfirst;
  do {
    if (ccb->fun == f) {
      if (ccfirst == ccb) {
	ccfirst = ccb->next;
	delete ccb;
	if (!ccfirst) return NULL;
	ccfirst->prev = NULL;
      } else {
	if ((ccb->prev->next = ccb->next))
	  ccb->next->prev = ccb->prev;
	delete ccb;
      }
      break;
    }
    ccb = ccb->next;
  } while (ccb);

  return ccfirst;
}

int
CindyCallback::execCallback(unsigned short *buf, int len)
{
  CindyCallback *cb = this;

  if (len<0) len = -len;
  while (cb) {
    (*(cb->fun))(buf,len,data);
    cb = cb->next;
  }

  return 0;
}

int
CindyNode::pack(short *root)
{
    short *dptr = root;
    CindyNode *snode = c;
    int sindex = 1;
    while (mark_) {
	if (snode->mark_) {
	    if (snode->tag == C) {
		dptr[0] = -sindex;
		dptr[1] = snode->pack(dptr+2);
		mark_--;
	    } else {	// snode->tag != C
		if ((mark_ == subnodes)&&(snode->typeCheck(subnodes))) {
		    dptr[-2] *= -1;
		    snode->dataCopy(dptr,subnodes);
		    int i = subnodes*snode->sizeOf(); 
		    for ( ; mark_ ; snode++,mark_-- ) snode->mark_=0;
		    return i;
		} else { // mark_ != subnodes || !snode->typeCheck(subnodes)
		    if ((mark_ > 1) && (c->typeCheck(subnodes))) {
			int i = 0;
			for ( ; mark_ ; snode++,sindex++ ) {
			    if (snode->mark_) {
				dptr[i++] = sindex;
				snode->dataCopy(dptr+i);
				i += snode->sizeOf();
				mark_--; snode->mark_--;
			    }
			}
			return -i;
		    } else { // mark_ == 1 || !c->typeCheck(subnodes)
			dptr[0] = sindex;
			dptr[1] = snode->sizeOf();
			snode->dataCopy(dptr+2);
			snode->mark_ = 0;
			mark_--;
		    }
		}
	    }
	    if (dptr[1] > 0) {
		dptr += dptr[1] + 2;
	    } else {
		dptr += -dptr[1] + 2;
	    }
	}
	sindex++;
	snode++;
    }

    return dptr-root;
}

int
CindyNode::typeCheck(int N)
{
  CindyNode *node = this;
  
  while (--N) {
    node++;
    if (tag != node->tag) return 0;
  }

  return 1;
}

CindyNode *
CindyNode::mark(int& dR)
{
    dR += 2;			// header size

    if (tag != C) {
	if (OK) *OK = 1;
	if (mark_) return NULL;
	dR += sizeOf();
	mark_ = 1;
    }

    if (up) {
	up->mark_++;
	if (up->mark_>1) return NULL;
    }

    return up;
}

void
CindyNode::dataCopy(short *ptr, int num)
{
  short *data = s;

  while (num--) {
    switch (tag) {
    case S:
    case US:
      *ptr++ = *data++;
      break;
    case I:
    case UI:
    case L:
    case UL:
      XDRcpy(ptr,(int *)data);
      ptr += INTINC;
      data += INTINC;
      break;
    case F:
      XDRcpy(ptr,(float *)data);
      ptr += FLOATINC;
      data += FLOATINC;
      break;
    case D:
      XDRcpy(ptr,(double *)data);
      ptr += DOUBLEINC;
      data += DOUBLEINC;
      break;
    default:
      break;
    }
  }
}

int
CindyNode::sizeOf()
{
  switch (tag) {
  case S:
  case US:
    return SHORTINC;
    break;
  case I:
  case UI:
  case L:
  case UL:
    return INTINC;
    break;
  case F:
    return FLOATINC;
    break;
  case D:
    return DOUBLEINC;
    break;
  default:
    return 0;
    break;
  }

  return 0;
}

void
CindyNode::clear()
{
  *OK = 0;
  switch (tag) {
  case S:
    *s = 0;
    break;
  case US:
    *us = 0;
    break;
  case I:
    *i = 0;
    break;
  case UI:
    *ui = 0;
    break;
  case L:
    *l = 0;
    break;
  case UL:
    *ul = 0;
    break;
  case F:
    *f = 0.0;
    break;
  case D:
    *d = 0.0;
    break;
  default:
    break;
  }
}


void
CindyNode::addCallback(EvDpFun f, char *d)
{
  if (callback) {
    if (new CindyCallback(f,d,callback)) return;
  }
  else
    callback = new CindyCallback(f,d);
}


void
CindyNode::cancelCallback(EvDpFun f)
{
  if (callback) callback = callback->cancel(f);
}
