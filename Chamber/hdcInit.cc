//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/Chamber/hdcInit.cc,v 2.3 2002-11-08 20:27:26 distler Exp $
//
// Parser to initialize OOPS horizontal driftchambers (HDCs)
//

#include <stdio.h>
#include <stdlib.h>
#include "hdcInit.h"

hdcInit::hdcInit(const char * initfile)
{
  yerrs = -1;

  if (NULL == (file = fopen(initfile,"r"))) {
    char tmpbuf[256];
    sprintf(tmpbuf,"hdcInit: '%s'",initfile);
    perror(tmpbuf);
    return;
  }
  lex = new hdcInitLEX(file);

  yerrs = 0;
  yyparse();

  delete lex;

  fclose(file);
  file = NULL;
}

int
hdcInit::yylex()
{
  yylloc.first_line = lex->lineno;
  int token	    = lex->yylex(&yylval);
  yylloc.last_line  = lex->lineno;
  yylloc.text	    = (char *)lex->yytext;

  return token;
}

void
hdcInit::yyerror(char *m)
{
  yerrs++;
  std::cerr << " - line " << yylloc.first_line << ": "
	    << m << " at token \'" << yylloc.text << '\'' << std::endl;

  if (yerrs>=20) exit(errno);
}

hdcInit::~hdcInit() { ; }

void
hdcInit::planeInit(PlaneDefault * init)
{
  if (!yerrs) memcpy(init, &planeDefault, sizeof(PlaneDefault));
}

#ifdef NEVER
int
main(int argv, char *argc[])
{
  if (argv != 2) return -1;

  PlaneDefault plane;
  for (int i = 0; i < 10000; i++) {
    hdcInit * init = new hdcInit(argc[1]);
    init->planeInit(&plane);
    delete init;
    std::cout << i << " :\t"
	      << plane.bintons[0] << '\t' 
	      << plane.bintons[1] << std::endl;
    delete plane.drifttimeTable;
  }

  return 0;
}
#endif
