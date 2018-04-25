//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                           mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B        tel.   (617) 253-6997
// Cambridge, MA 02139                          fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/UniqRunDB.h,v 2.2 2002-12-12 12:24:12 distler Exp $
//
// remove duplicate lines from run.db
//

#ifndef __UniqRunDb_h__
#define __UniqRunDb_h__

#include <errno.h>
#include <iostream>
#include "UniqRunDB.parse.h"
#include "UniqRunDB.lex.h"

class UniqRunDB : public UniqRunDbYACC {
  FILE *file;
  int yerrs;
  UniqRunDbLEX *uniqRunDbLEX;
  virtual int yylex();
  virtual void yyerror(char *m);
public:
  UniqRunDB(char *cardfile, std::ostream *output);
  virtual ~UniqRunDB();
};

#endif /* __UniqRunDb_h__ */
