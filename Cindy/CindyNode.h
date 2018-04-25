//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyNode.h,v 4.14 2002-12-12 12:24:41 distler Exp $
//
// Header File fuer class CindyNode
//

#ifndef __cindy_node__
#define __cindy_node__

#include "Cindy/EventDispatch.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cstddef>
#include <malloc.h>
#ifdef HAVE_REGEX_H
#include <regex.h>
#endif

class CindyCallback {
  EvDpFun fun;
  char *data;
  CindyCallback *prev;
  CindyCallback *next;
public:
  CindyCallback(EvDpFun, char *);
  CindyCallback(EvDpFun, char *, CindyCallback *);
  CindyCallback *getNext() { return next; }
  int execCallback(unsigned short *, int);
  CindyCallback *cancel(EvDpFun);
  virtual ~CindyCallback() { ; }
};

class CindyNode {
public:
  enum Tag { S, US, I, UI, L, UL, F, D, C, V };
private:
  char *name;
  CindyNode *up;
  union {
    short *s;
    unsigned short *us;
    int *i;
    unsigned int *ui;
    long *l;
    unsigned long *ul;
    float *f;
    double *d;
    CindyNode *c;
    void *v;
  };
  unsigned short *OK;
  CindyCallback *callback;
  ptrdiff_t offset;
  int subnodes, mark_;
  Tag tag;
  static int fieldwidth;
  static char *pattern;
#ifdef HAVE_REGEX_H
  static regex_t *regex;
#endif
  int decodePrint(unsigned short *);
  int decodePrint(unsigned short *, const char *);
  int analyse(unsigned short *);
  void dataCopy(short *ptr, int num=1);
  int typeCheck(int);
public:
  CindyNode();
  void clear();
  void value(const char *nam);
  void value(CindyNode *br) { if (tag != C) { tag = C; c = br; } subnodes++; }
  void pointer(CindyNode *u) { up = u; }
  void pointer(CindyNode *u, short *val) { tag = S; s = val; up = u; }
  void pointer(CindyNode *u, unsigned short *val) { tag = US;us = val;up = u; }
  void pointer(CindyNode *u, int *val) { tag = I; i = val; up = u; }
  void pointer(CindyNode *u, unsigned int *val) { tag = UI; ui = val; up = u; }
  void pointer(CindyNode *u, long *val) { tag = L; l = val; up = u; }
  void pointer(CindyNode *u, unsigned long *val) { tag = UL;ul = val; up = u; }
  void pointer(CindyNode *u, float *val) { tag = F; f = val; up = u; }
  void pointer(CindyNode *u, double *val) { tag = D; d = val; up = u; }
  void rootptr(char *, unsigned short *, CindyNode **);
  int  fastAnalyse(unsigned short *, int);
  int  analyse(unsigned short *, int);
  int  analyse(int, unsigned short *);
  int  decode(unsigned short *, int);
  int  decode(int, unsigned short *);
  int  decode(unsigned short *, int, const char *);
  int  decode(int, unsigned short *, const char *);
  int  pack(short *root);
  int  sizeOf();
  Tag  typeOf() { return tag; }
  static void setFieldWidth(int fw) { fieldwidth = fw; }
  const char *tellname() { return name; }
  void addCallback(EvDpFun, char *);
  void cancelCallback(EvDpFun);
#ifdef HAVE_REGEX_H
  static void setRegex(const char *rx);
#endif
  static void setPattern(const char *pat);
  unsigned short ok() { return (OK ? *OK : 0); }
  caddr_t addrOf() { return (caddr_t)s; }
  CindyNode *submatch(char *);
  CindyNode *mark(int&);
  ~CindyNode(void);
};

const int decodeFieldWidth = 69;
const int SHORTINC	= sizeof(short)	/ sizeof(short);
const int INTINC	= sizeof(int)	/ sizeof(short);
const int LONGINC	= sizeof(long)	/ sizeof(short);
const int FLOATINC	= sizeof(float)	/ sizeof(short);
const int DOUBLEINC	= sizeof(double)/ sizeof(short);

#endif
