//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                           mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B        tel.   (617) 253-6997
// Cambridge, MA 02139                          fax    (617) 258-5440
//
// $Id: UniqRunDB.cc 2354 2009-11-13 13:58:04Z merkel $
//
// remove duplicate lines from run.db
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <fstream>
#include <iosfwd>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif
#include "UniqRunDB.h"

UniqRunDB::UniqRunDB(char *rundb, std::ostream *output)
{
  out = output;
  if (!strcmp(rundb,"-"))
    file=stdin;
  else if (NULL == (file = fopen(rundb, "r"))) {
    char tmpbuf[256];
    sprintf(tmpbuf, "UniqRunDB: '%s'", rundb);
    perror(tmpbuf);
    exit(errno);
  }
  uniqRunDbLEX = new UniqRunDbLEX(file);
  yerrs = 0;

  yyparse();
}

int
UniqRunDB::yylex()
{
  yylloc.first_line = uniqRunDbLEX->lineno;
  int token	    = uniqRunDbLEX->yylex(&yylval);
  yylloc.last_line  = uniqRunDbLEX->lineno;
  yylloc.text	    = (char *)uniqRunDbLEX->yytext;

  return token;
}

void
UniqRunDB::yyerror(char *m)
{
  yerrs++;
  std::cerr << " - line " << yylloc.last_line << ": "
	    << m << " at token \'" << yylloc.text << '\'' << std::endl;

  if (yerrs>=20) exit(errno);
}

UniqRunDB::~UniqRunDB()
{
  if (file) fclose(file);
}

int
main(int argc, char* argv[])
{

  char opt, *input="run.db";
  std::ostream *out = &std::cout;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"output",    1, NULL, 'o'},
    {"help",      0, NULL, 'h'},
    {"input",     1, NULL, 'i'},
    {"directory", 1, NULL, 'C'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "o:i:hC:",lo,NULL))!=EOF) {
#else
  while ((opt = getopt(argc, argv, "o:i:hC:")) != EOF) {
#endif

    switch (opt) {
    case '?':
    case ':':
    case 'h': std::cout << "Usage: " << argv[0]
			<< " [-i input] [-o output} [-h] [-C directory]"
			<< std::endl;
              exit(0);
              break;
    case 'C': if (chdir(optarg));               break;
    case 'i': input = optarg;                   break;
    case 'o': out = new std::ofstream(optarg);  break;
    }
  }

  UniqRunDB uniqRunDB(input, out);

  return 0;
}
