//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCompiler.h,v 4.9 2002-12-12 12:24:41 distler Exp $
//
// Header File fuer den Cindy++ Compiler
//

#ifndef __CindyCompiler__
#define __CindyCompiler__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cstring>
#include <cstdio>
#include <errno.h>
#include "CindyYACC.h"
#include "CindyLEX.h"

class CindyCompiler : public CindyYACC {
  FILE *input;
  int yerrs;
  CindyLEX *cindyLEX;
public:
  virtual int yylex();
  virtual void yyerror(char *m);
  CindyCompiler(FILE *in)
    { cindyLEX = new CindyLEX(input=in); yerrs = 0; };
  CindyCompiler(int fd)
    { cindyLEX = new CindyLEX(input=fdopen(fd, "r")); yerrs = 0; };
  virtual ~CindyCompiler() { fclose(input); }
};

const int MAXLEVEL = 32;

class CindyCount {
  int level[MAXLEVEL];
  size_t longestName;
  int position;
public:
  CindyCount() { reset(); }
  void reset()
    { longestName = position = 0; memset(level, 0, MAXLEVEL*sizeof(int)); }
  void inc(int lvl) { if ((0 <= lvl) && (lvl < MAXLEVEL)) level[lvl]++; }
  void inc(int lvl, int size)
    { if ((0 <= lvl) && (lvl < MAXLEVEL)) level[lvl]++; position += size; }
  int get_level(int lvl)
    { if ((0 <= lvl) && (lvl < MAXLEVEL)) return level[lvl]; return 0; }
  void check(const char *name)
    { longestName = (strlen(name)>longestName) ? strlen(name) : longestName; }
  int length() { return longestName; }
  int get_position() { return position; }
  void position_align(int align)
    { position = ((position + align - 1) / align) * align; }
};
  
inline int
CindyCompiler::yylex()
{
  yylloc.first_line = cindyLEX->lineno;
  int token = cindyLEX->yylex(&yylval);
  yylloc.last_line = cindyLEX->lineno;
  yylloc.text = (char *)cindyLEX->yytext;
  return token;
}

inline void
CindyCompiler::yyerror(char *m)
{
  yerrs++;
  fprintf(stderr, "%d: %s at token '%s'\n", yylloc.first_line, m, yylloc.text);

  if (yerrs>=20) exit(errno);
}

#endif
