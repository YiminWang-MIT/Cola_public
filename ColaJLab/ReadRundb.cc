//
// Created 1996 by      Harald Merkel
//
//                      A1 Collaboration
//                      Institut fuer Kernphysik
//                      Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/ReadRundb.cc,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//

#define _DEFAULT_SOURCE    1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_BSD_BSD_H
#include <bsd/bsd.h>
#endif /* HAVE_BSD_BSD_H */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RunParser.parse.cc"

void RUNDATABASE::yyerror (char *s)
{
  printf ("%s:%d: %s\n", filename, linenumber, s);
}


int RUNDATABASE::yylex ()
{
  int c;
  while (((c = fgetc(parsefile)) == ' ') || (c == '\t'));
  if (c == '\\'){ c = fgetc(parsefile); linenumber++; return yylex();}
  if (c == '!') { c = fgetc(parsefile);
                  if (c == '=') return UNEQUAL;
		  ungetc(c, parsefile); c = '!'; }
  if (c == '=') { c = fgetc(parsefile);
		  if (c == '=') return EQUAL;
		  ungetc(c, parsefile); c = '='; }
  if (c == '<') { c = fgetc(parsefile);
		  if (c == '=') return LE;
		  ungetc(c, parsefile); c = '<'; }
  if (c == '>') { c = fgetc(parsefile);
		  if (c == '=') return GE;
		  ungetc(c, parsefile); c = '>'; }
  if (c == '&') { c = fgetc(parsefile);
		  if (c == '&') return AND;
		  ungetc(c, parsefile); c = '&'; }
  if (c == '|') { c = fgetc(parsefile);
		  if (c == '|') return OR;
		  ungetc(c, parsefile); c = '|'; }
  if (c == '/') { c = fgetc(parsefile);
		  if (c == '/') {
		    while ((c != '\n') && (c != EOF)) c = fgetc(parsefile); 
		    return c;
		  }
		  ungetc(c, parsefile); c = '/'; }

  if (c == '!') { while ((c != '\n') && (c != EOF)) c = fgetc(parsefile);
                  return c; }
  if (c == '"') { int i = c = 0;
		  char function[256];
		  while  (c != '"' && c != EOF)
		    function[i++] = c = fgetc(parsefile);
		  function[i-1]=0;
		  yylval = (double *) new char[sizeof(function)+1];
		  strcpy((char *) yylval, function); 
		  return STRING; }
  if (isdigit (c)) {
    ungetc(c, parsefile);
    yylval = new double;
    fscanf(parsefile, "%lf", yylval);
    return NUM;
  }
  if (islower(c) || isupper(c) || (c == '.') || (c == '@')) {
    int i = 0;
    char function[255];
    while (isalnum(c) || (c == '.') || (c == '_') || (c == '@')) {
      function[i++] = c;
      c = fgetc(parsefile);
    }

    function[i] = 0;
    ungetc(c, parsefile);

    i = -1; 
    while(lname[++i].Name) 
      if (!strcmp(lname[i].Name,function)) {
	yylval = (double *) lname[i].Name;
	return NAME;
      }
  }
  if (c == EOF) return 0;

  return c;
}

void dumprundb(FILE *fp)
{
  fprintf(fp, "//---------------------------------------------\n");
  fprintf(fp, "//                Run - Database               \n");
  fprintf(fp, "//---------------------------------------------\n");
  fprintf(fp, "[%s]\n",lasttime);
  int i = -1;
  while (lname[++i].Name) {
    int l = fprintf(fp, "\t%s = ", lname[i].Name);
    if (lname[i].type == 1)
      fprintf(fp, "%g\n", *(double *)lname[i].addr);
    if (lname[i].type == -1)
      fprintf(fp, "\"%s\"\n", *(char **)lname[i].addr);
    if (lname[i].type > 1) {
      l += fprintf(fp, "{");
      for (int j = 0; j < lname[i].type; j++) {
	l += fprintf(fp, "%g", ((double *)lname[i].addr)[j]);
	if (j == lname[i].type-1) fprintf(fp, "}\n");
	else if (l>55) l=fprintf(fp, ",\n\t\t"); else l+=fprintf(fp, ", ");
      }
    }
  }
  fprintf(fp, "//---------------------------------------------\n");
}

void readRunDatabase(char *name, time_t time)
{
  RUNDATABASE *pr =new RUNDATABASE;

  static int putenvresult = putenv("TZ=EST5EDT"); // Timezone US Eastern
  tzset();
  filename="-";
  if (!strcmp(name,"-")) parsefile = stdin; 
  else {
    filename=name;
    char command[255]; sprintf(command,GNUCPP " %s",name);
    parsefile = popen(command,"r");
  }
  if (!parsefile) {
    fprintf(stderr,"Can't open input file \"%s\"\n", name);
    exit(-1);
  }

  stoptime = time;
  setup = NULL;
  pr->yyparse();
  while (!feof(parsefile)) fgetc(parsefile); // clean exit of gnucpp subprocess
  fclose(parsefile);

  if (getenv("VERBOSE")!=NULL) dumprundb(stderr);

  delete pr;
}

void readRunDatabase(char *name, char *set)
{
  RUNDATABASE *pr =new RUNDATABASE;

  filename = "-";
  if (!strcmp(name,"-")) parsefile = stdin; 
  else {
    filename = name;
    char command[255];
    sprintf(command,GNUCPP " %s",name);
    parsefile = popen(command,"r");
  }
  if (!parsefile) {
    fprintf(stderr,"Can't open input file \"%s\"\n", name);
    exit(-1);
  }
  stoptime = -1;
  setup = set;
  rundb.setup = "No line 'Setup = \"...\"' found";
  pr->yyparse();
  while (!feof(parsefile)) fgetc(parsefile); // clean exit of gnucpp subprocess
  fclose(parsefile);

  if (getenv("VERBOSE")!=NULL) dumprundb(stderr);

  delete pr;
}












