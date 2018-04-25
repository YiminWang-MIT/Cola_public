//								-*-c++-*-
// Copyright (c) 1995 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCard.cc,v 4.9 2002-12-12 12:24:41 distler Exp $
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CindyCard.h"

CindyCard::CindyCard(char *cardfile)
{
  if (NULL == (file = fopen(cardfile,"r"))) {
    char tmpbuf[256];
    sprintf(tmpbuf,"CindyCard: '%s'",cardfile);
    perror(tmpbuf);
    exit(errno);
  }
  cindyCardLEX = new CindyCardLEX(file);

  beam = 0.0;
  memset(&coinc,0,sizeof(Coinc));
  memset(&specA,0,sizeof(Spec));
  memset(&specB,0,sizeof(Spec));
  memset(&specC,0,sizeof(Spec));
  yerrs = 0;

  yyparse();
}

int
CindyCard::yylex()
{
  yylloc.first_line = cindyCardLEX->lineno;
  int token	    = cindyCardLEX->yylex(&yylval);
  yylloc.last_line  = cindyCardLEX->lineno;
  yylloc.text	    = (char *)cindyCardLEX->yytext;

  return token;
}

void
CindyCard::yyerror(char *m)
{
  yerrs++;
  std::cerr << " - line " << yylloc.first_line << ": "
	    << m << " at token \'" << yylloc.text << '\'' << std::endl;

  if (yerrs>=20) exit(errno);
}

CindyCard::~CindyCard()
{
  if (file) fclose(file);
}
